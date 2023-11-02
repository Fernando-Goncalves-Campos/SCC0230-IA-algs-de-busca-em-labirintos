from PIL import Image
import os

directory = os.path.dirname(__file__)
ppmPath = directory + r'/ppmFiles'
pngPath = directory + r'/pngFiles'

for f in os.listdir(ppmPath):
  im = Image.open(os.path.join(ppmPath, f))

  newIm = im.resize((1024, 1024), Image.Resampling.NEAREST)
  newIm.save(os.path.join(pngPath, os.path.splitext(f)[0] + '.png'))