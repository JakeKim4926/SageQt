import math
import sys
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

REPO_ROOT = Path(__file__).resolve().parents[2]
FONT_PATH = REPO_ROOT / 'SageQt' / 'resources' / 'GmarketSansTTFBold.ttf'
OUTPUT_DIR = REPO_ROOT / 'SageQt' / 'resources' / 'icons'

DESIGN_SIZE = 1024
RENDER_SIZE = 4096
CENTER = DESIGN_SIZE / 2

OUTER_RADIUS = 472
RING_RADIUS = 456
DOT_ORBIT_RADIUS = 424
DOT_RADIUS = 9
DOT_COUNT = 36
FACE_RADIUS = 392
FACE_OUTLINE_WIDTH = 8
BAND_TOP = 612
BAND_BOTTOM = 668
BAND_LEFT = 150
BAND_RIGHT = 874
LABEL_TEXT = 'S.A.G.E'
LABEL_SIZE = 168
LABEL_CENTER_Y = 520

COLOR_OUTER = (226, 210, 170, 255)
COLOR_RING = (244, 234, 208, 255)
COLOR_DOT = (214, 196, 152, 255)
COLOR_FACE = (252, 248, 236, 255)
COLOR_FACE_OUTLINE = (222, 206, 166, 255)
COLOR_BAND = (232, 206, 140, 255)
COLOR_LABEL = (126, 98, 58, 255)

ICO_SIZES = [(16, 16), (24, 24), (32, 32), (48, 48), (64, 64), (128, 128), (256, 256)]
ICNS_SIZES = [(16, 16), (32, 32), (64, 64), (128, 128), (256, 256), (512, 512), (1024, 1024)]
LINUX_SIZE = 256


def scale(value):
    return value * RENDER_SIZE / DESIGN_SIZE


def circle_box(radius, center_x=CENTER, center_y=CENTER):
    return [scale(center_x - radius), scale(center_y - radius), scale(center_x + radius), scale(center_y + radius)]


def render_master():
    image = Image.new('RGBA', (RENDER_SIZE, RENDER_SIZE), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)
    draw.ellipse(circle_box(OUTER_RADIUS), fill=COLOR_OUTER)
    draw.ellipse(circle_box(RING_RADIUS), fill=COLOR_RING)
    for index in range(DOT_COUNT):
        angle = 2 * math.pi * index / DOT_COUNT
        dot_x = CENTER + DOT_ORBIT_RADIUS * math.cos(angle)
        dot_y = CENTER + DOT_ORBIT_RADIUS * math.sin(angle)
        draw.ellipse(circle_box(DOT_RADIUS, dot_x, dot_y), fill=COLOR_DOT)
    draw.ellipse(circle_box(FACE_RADIUS), fill=COLOR_FACE)
    draw.ellipse(circle_box(FACE_RADIUS), outline=COLOR_FACE_OUTLINE, width=int(scale(FACE_OUTLINE_WIDTH)))

    band = Image.new('RGBA', image.size, (0, 0, 0, 0))
    ImageDraw.Draw(band).rectangle([scale(BAND_LEFT), scale(BAND_TOP), scale(BAND_RIGHT), scale(BAND_BOTTOM)],
                                   fill=COLOR_BAND)
    face_mask = Image.new('L', image.size, 0)
    ImageDraw.Draw(face_mask).ellipse(circle_box(FACE_RADIUS), fill=255)
    band_mask = Image.composite(band.getchannel('A'), Image.new('L', image.size, 0), face_mask)
    image.paste(band, (0, 0), band_mask)

    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype(str(FONT_PATH), int(scale(LABEL_SIZE)))
    left, top, right, bottom = draw.textbbox((0, 0), LABEL_TEXT, font=font)
    draw.text((scale(CENTER) - (right - left) / 2 - left, scale(LABEL_CENTER_Y) - (bottom - top) / 2 - top), LABEL_TEXT,
              font=font, fill=COLOR_LABEL)
    return image.resize((DESIGN_SIZE, DESIGN_SIZE), Image.LANCZOS)


def main():
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    master = render_master()
    master.save(OUTPUT_DIR / 'sageqt-1024.png')
    master.resize((LINUX_SIZE, LINUX_SIZE), Image.LANCZOS).save(OUTPUT_DIR / 'sageqt-256.png')
    master.save(OUTPUT_DIR / 'sageqt.ico', sizes=ICO_SIZES)
    master.save(OUTPUT_DIR / 'sageqt.icns', sizes=ICNS_SIZES)
    print(f'icons written to {OUTPUT_DIR}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
