import json
import subprocess
import sys
import time
from pathlib import Path

import numpy
import pyautogui
from PIL import ImageGrab

CAPTION_COLOR = (242, 238, 231)
WORK_AREA_COLOR = (248, 246, 241)
COLOR_TOLERANCE = 3
CAPTION_HEIGHT = 40
CAPTION_MIN_ROW_PIXELS = 200
CAPTION_MIN_ROWS = 30
WAIT_SECONDS = 30
SETTLE_SECONDS = 2
DRAG_GRIP = (60, 20)
DRAG_OFFSET = (150, 100)
DRAG_SECONDS = 1.0
HEADER_ROW_HEIGHT = 56
HEADER_TOTAL_HEIGHT = 57
CONTENT_PAD_X = 24
LOGIN_BUTTON_HALF_WIDTH = 34


def grab():
    return ImageGrab.grab()


def find_caption(image):
    return find_band(image, CAPTION_COLOR)


def find_band(image, color):
    pixels = numpy.asarray(image.convert('RGB')).astype(int)
    distance = numpy.abs(pixels - numpy.array(color)).max(axis=2)
    matches = distance <= COLOR_TOLERANCE
    band_rows = numpy.nonzero(matches.sum(axis=1) >= CAPTION_MIN_ROW_PIXELS)[0]
    if band_rows.size < CAPTION_MIN_ROWS:
        return None
    breaks = numpy.nonzero(numpy.diff(band_rows) != 1)[0]
    runs = numpy.split(band_rows, breaks + 1)
    caption_rows = max(runs, key=len)
    if caption_rows.size < CAPTION_MIN_ROWS:
        return None
    columns = numpy.nonzero(matches[caption_rows].any(axis=0))[0]
    return [int(columns.min()), int(caption_rows.min()), int(columns.max()), int(caption_rows.max())]


def wait_for_caption():
    deadline = time.time() + WAIT_SECONDS
    while time.time() < deadline:
        image = grab()
        caption = find_caption(image)
        if caption is not None:
            return image, caption
        time.sleep(1)
    return grab(), None


def main():
    app_path, out_dir, prefix = sys.argv[1], Path(sys.argv[2]), sys.argv[3]
    out_dir.mkdir(parents=True, exist_ok=True)
    pyautogui.FAILSAFE = False
    result = {'prefix': prefix}

    with open(out_dir / f'{prefix}-app.log', 'w') as log:
        app = subprocess.Popen([app_path], stdout=log, stderr=subprocess.STDOUT)
        try:
            time.sleep(SETTLE_SECONDS)
            image, before = wait_for_caption()
            image.save(out_dir / f'{prefix}-1-dialog.png')
            result['screenPixels'] = list(image.size)
            result['screenPoints'] = list(pyautogui.size())
            result['captionBefore'] = before
            if before is None:
                return result

            scale = image.size[0] / pyautogui.size()[0]
            result['scale'] = scale
            start_x = (before[0] + DRAG_GRIP[0] * scale) / scale
            start_y = (before[1] + DRAG_GRIP[1] * scale) / scale
            pyautogui.moveTo(start_x, start_y)
            pyautogui.mouseDown()
            pyautogui.moveTo(start_x + DRAG_OFFSET[0], start_y + DRAG_OFFSET[1], duration=DRAG_SECONDS)
            pyautogui.mouseUp()
            time.sleep(SETTLE_SECONDS)

            image = grab()
            image.save(out_dir / f'{prefix}-2-dragged.png')
            after = find_caption(image)
            result['captionAfter'] = after
            if after is not None:
                moved = [(after[0] - before[0]) / scale, (after[1] - before[1]) / scale]
                result['movedPoints'] = moved
                result['moved'] = abs(moved[0] - DRAG_OFFSET[0]) <= CAPTION_HEIGHT and abs(moved[1] - DRAG_OFFSET[1]) <= CAPTION_HEIGHT

            pyautogui.press('enter')
            time.sleep(SETTLE_SECONDS)
            image = grab()
            image.save(out_dir / f'{prefix}-3-after-enter.png')
            result['closedByEnter'] = find_caption(image) is None

            work_area = find_band(image, WORK_AREA_COLOR)
            result['workArea'] = work_area
            if work_area is None:
                return result
            login_x = (work_area[2] - (CONTENT_PAD_X + LOGIN_BUTTON_HALF_WIDTH) * scale) / scale
            login_y = (work_area[1] - (HEADER_TOTAL_HEIGHT - HEADER_ROW_HEIGHT / 2) * scale) / scale
            pyautogui.click(login_x, login_y)
            image, login_caption = wait_for_caption()
            image.save(out_dir / f'{prefix}-4-login.png')
            result['loginOpened'] = login_caption is not None
            if login_caption is None:
                return result

            pyautogui.press('enter')
            time.sleep(SETTLE_SECONDS)
            grab().save(out_dir / f'{prefix}-5-login-empty-id.png')
            pyautogui.press('escape')
            time.sleep(SETTLE_SECONDS)
            result['loginClosedByEscape'] = find_caption(grab()) is None
            return result
        finally:
            app.kill()
            app.wait()


if __name__ == '__main__':
    outcome = main()
    print(json.dumps(outcome, indent=2))
    Path(sys.argv[2], f'{sys.argv[3]}-result.json').write_text(json.dumps(outcome, indent=2))
