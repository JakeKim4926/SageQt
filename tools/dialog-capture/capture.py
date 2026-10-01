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
TAB_INDICATOR_COLOR = (154, 107, 63)
TAB_INDICATOR_MIN_PIXELS = 40
TAB_INDICATOR_MAX_ROWS = 3
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
TAB_ROW_HEIGHT = 40
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


def find_tab_indicator(image, work_area, scale):
    pixels = numpy.asarray(image.convert('RGB')).astype(int)
    region = pixels[:, work_area[0]:work_area[2] + 1]
    matches = numpy.abs(region - numpy.array(TAB_INDICATOR_COLOR)).max(axis=2) <= COLOR_TOLERANCE
    rows = numpy.nonzero(matches.sum(axis=1) >= TAB_INDICATOR_MIN_PIXELS * scale)[0]
    if rows.size == 0:
        return None
    runs = numpy.split(rows, numpy.nonzero(numpy.diff(rows) != 1)[0] + 1)
    thin_runs = [run for run in runs if run.size <= TAB_INDICATOR_MAX_ROWS * scale]
    if not thin_runs:
        return None
    indicator_rows = thin_runs[0]
    columns = numpy.nonzero(matches[indicator_rows].any(axis=0))[0]
    return [int(work_area[0] + columns.min()), int(indicator_rows.min()), int(work_area[0] + columns.max()), int(indicator_rows.max())]


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
            login_y = (work_area[1] - (TAB_ROW_HEIGHT + HEADER_TOTAL_HEIGHT - HEADER_ROW_HEIGHT / 2) * scale) / scale
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
            image = grab()
            result['loginClosedByEscape'] = find_caption(image) is None

            indicator = find_tab_indicator(image, work_area, scale)
            result['tabIndicator'] = indicator
            if indicator is None:
                return result
            tab_width = indicator[2] - indicator[0] + 1
            pyautogui.click((indicator[0] + tab_width * 1.5) / scale, (indicator[1] - TAB_ROW_HEIGHT * scale / 2) / scale)
            time.sleep(SETTLE_SECONDS)
            image = grab()
            image.save(out_dir / f'{prefix}-6-result-tab.png')
            moved_indicator = find_tab_indicator(image, work_area, scale)
            result['resultTabSelected'] = moved_indicator is not None and moved_indicator[0] > indicator[2]

            pyautogui.click((indicator[0] + tab_width * 2.5) / scale, (indicator[1] - TAB_ROW_HEIGHT * scale / 2) / scale)
            time.sleep(SETTLE_SECONDS)
            image = grab()
            image.save(out_dir / f'{prefix}-7-history-tab.png')
            history_indicator = find_tab_indicator(image, work_area, scale)
            result['historyTabSelected'] = history_indicator is not None and history_indicator[0] > indicator[2] + tab_width
            return result
        finally:
            app.kill()
            app.wait()


if __name__ == '__main__':
    outcome = main()
    print(json.dumps(outcome, indent=2))
    Path(sys.argv[2], f'{sys.argv[3]}-result.json').write_text(json.dumps(outcome, indent=2))
