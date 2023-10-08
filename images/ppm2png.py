from PIL import Image
from os import listdir
from os.path import dirname, join, splitext

directory = dirname(__file__)
ppmPath = directory + r'/ppmFiles'
pngPath = directory + r'/pngFiles'

for f in listdir(ppmPath):
  im = Image.open(join(ppmPath, f))

  print(f)
  print(im.size)
  newIm = im.resize((1000, 1000))
  print(newIm.size)
  newIm.save(join(pngPath, splitext(f)[0] + '.png'))