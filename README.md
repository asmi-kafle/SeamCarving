# SeamCarving

This is an image manipulation program written in C++. This program first imports a .ppm file into a 2D array of pixels. It creates another array to calculate and store the energy of pixels. Then seams are calculated in the horizontal and vertical directions starting from each edge pixel. A seam is the set of vertical or horizontal pixels whose total energy (when summed) is a minimum. In essence, the energy is the measure of a pixel's similarity to its neighbors, and so the seam with the minimum energy across the whole image is likely to not have any "interesting" features. Then based on user input of the target width and height, the image is resized by adding/deleting seams ofminimum energy. The ouptut is written to a new .ppm file and exported to the root directoy.

The project also comes with one sample .ppm image for testing purposes `./Fishing175X117.ppm`

**To run the code:**

```bash
# Run the make command:
$  `make all`
# run the output file:
$   `./seamTest`
# enter the image file name:
# ex: Fishing175X117.ppm
# note that the width and the height are given in the file name (i.e. 175 and 117 respectively) and the target width and height are upto the user.
$ specify width, height, target width and target height
# Open the new image to see the change (IDE only shows it in pixels). Use application like GIMP to view the image.
```
