#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

int main()
{
  string filename;
  int width = 0;
  int height = 0;
  int targetWidth = 0;
  int targetHeight = 0;

  // taking user input from terminal
  cout << "Input filename: ";
  cin >> filename;

  cout << "Input width, height, target width, and target height: ";

  //validation of user input
  cin >> width;
  if (!cin)
  {
    cout << "Error: width is a non-integer value" << endl;
    return -1;
  }
  cin >> height;
  if (!cin)
  {
    cout << "Error: height is a non-integer value" << endl;
    return -1;
  }
  if (validation(width, "width", 0, "zero") && validation(height, "height", 0, "zero"))
  {

    cin >> targetWidth;
    if (!cin)
    {
      cout << "Error: target width is a non-integer value" << endl;
      return -1;
    }
    cin >> targetHeight;
    if (!cin)
    {
      cout << "Error: target height is a non-integer value" << endl;
      return -1;
    }

    if (validation(targetHeight, "target height", height, "height") && validation(targetWidth, "target width", width, "width"))
    {
      //the integers are validated so we can continue

      cout << "made it through the validations!" << endl;

      int originalWidth = width; // need to delete image array at end of program

      Pixel **image = createImage(width, height); // create array of size that we need

      // Loading the data and starting the carving process
      if (image != nullptr)
      {
        if (loadImage(filename, image, width, height))
        {
          cout << "Start carving..." << endl;
          while (width > targetWidth || height > targetHeight)
          {
            if (width > targetWidth)
            {
              int *minVerticalSeam = findMinVerticalSeam(image, width, height);

              removeVerticalSeam(image, width, height, minVerticalSeam);
              width = width - 1;
            }
            if (height > targetHeight)
            {
              int *minHorizontalSeam = findMinHorizontalSeam(image, width, height);
              removeHorizontalSeam(image, width, height, minHorizontalSeam);
              height = height - 1;
            }
          }

          //saving output image with filename that corresponds to the new size using a string stream
          stringstream ss;

          ss << "carved" << targetWidth << "X" << targetHeight << "." << filename;
          outputImage(ss.str().c_str(), image, targetWidth, targetHeight);
        }
        else
        {
          //failed to open input file
          return -1;
        }
        // call last to remove the memory from the heap
        deleteImage(image, originalWidth);
      }
      else
      {
        // else the image is the null pointer
        cout << "image == nullptr" << endl;
      }
    }
    else
    {
      //exit the program becuase the target width or target height is not valid
      return -1;
    }
  }
  else
  {
    //exit the program becuase the width or height is not valid
    return -1;
  }
}
