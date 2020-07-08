#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

//function to validate that the integers entered (width, height, target width and target height) are greater than 0
bool validation(int param, std::string paramName, int target, std::string targetName)
{
  //this handles the validation for the original height and width
  if (target == 0)
  {
    if (param <= target)
    {
      //if there is an error with the validation - then the standard out is printed and a false is returned.
      cout << "Error: " << paramName << " must be greater than 0. You entered " << param << endl;
      return false;
    }
  }
  //this handles the validation for the target height and width
  if (target != 0)
  {
    if (param > target)
    {
      //if there is an error with the validation - then the standard out is printed and a false is returned.
      cout << "Error: " << paramName << " must be less than " << targetName << ", " << param << " is greater than " << target << endl;
      return false;
    }
    if (param <= 0)
    {
      cout << "Error: " << paramName << " must be greater than 0. You entered " << param << endl;
      return false;
    }
  }
  return true;
}

//fuction to validate the width and height as input by user and as listed in the ppm file
bool inputValidation(int fileWidth, int targetWidth, int fileHeight, int targetHeight)
{
  if (fileWidth != targetWidth)
  {
    cout << "ERROR: input width (" << targetWidth << ") does not match value in file (" << fileWidth << ")" << endl;
    return false;
  }
  if (fileHeight != targetHeight)
  {
    cout << "ERROR: input height (" << targetHeight << ") does not match value in file (" << fileHeight << ")" << endl;
    return false;
  }
  return true;
}

//function to create a 2D array to store pixel data for manipulation
Pixel **createImage(int width, int height)
{
  cout << "Start createImage... " << endl;

  // Create a one dimensional array on the heap of pointers to Pixels
  //    that has width elements (i.e. the number of columns)
  Pixel **image = new Pixel *[width];

  bool fail = false;

  for (int i = 0; i < width; ++i)
  { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];

    if (image[i] == nullptr)
    { // failed to allocate
      fail = true;
    }
  }

  if (fail)
  { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    cout << "failed to allocte a block for the image" << endl;
    for (int i = 0; i < width; ++i)
    {
      delete[] image[i]; // deleting nullptr is not a problem
    }
    delete[] image; // dlete array of pointers
    return nullptr;
  }

  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = {-1, -1, -1};
    }
  }
  // cout << "End createImage... " << image << endl;
  return image;
}

// deleting temp images
void deleteImage(Pixel **image, int width)
{
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i = 0; i < width; ++i)
  {
    delete[] image[i]; // delete each individual array placed on the heap
  }
  delete[] image;
  image = nullptr;
}

//utility function for creating seam
int *createSeam(int length)
{
  int *Seam = new int[length];
  return Seam;
}

//utility function for deleting seam
void deleteSeam(int *seam)
{
  delete[] seam;
}

// Method to load the ppm image as a 2D-array of pixel values for further processing
bool loadImage(string filename, Pixel **image, int width, int height)
{
  cout << " Loading image ... " << endl;
  std::string fileType;
  int fileWidth, fileHeight, maxPixelValue = 0;
  //takes filename and reads it line by line
  //line 2 has information about the width  = number of columns and height  = number of rows
  ifstream inputFile(filename);
  if (inputFile.is_open())
  {
    inputFile >> fileType;
    if (fileType == "P3" || fileType == "p3")
    {
      inputFile >> fileWidth;
      inputFile >> fileHeight;
      inputFile >> maxPixelValue;

      if (fileWidth != 0 && fileHeight != 0 && maxPixelValue != 0)
      {
        if (inputValidation(fileWidth, width, fileHeight, height))
        {
          for (int x = 0; x < height; ++x)
          {
            if (inputFile.eof())
            {
              cout << "ERROR: not enough color values" << endl;
              return false;
            }
            for (int y = 0; y < width; ++y)
            {

              inputFile >> image[y][x].r;
              if (inputFile.eof())
              {
                cout << "ERROR: not enough color values 1" << endl;
                return false;
              }
              if (image[y][x].r == -1)
              {
                cout << "ERROR: read non-integer value" << endl;
                return false;
              }
              if (image[y][x].r < 0 || image[y][x].r > maxPixelValue)
              {
                cout << "ERROR: invalid color value " << image[y][x].r << endl;
                return false;
              }

              inputFile >> image[y][x].g;
              if (inputFile.eof())
              {
                cout << "ERROR: not enough color values 2" << endl;
                return false;
              }
              if (image[y][x].g == -1)
              {
                cout << "ERROR: read non-integer value" << endl;
                return false;
              }
              if (image[y][x].g < 0 || image[y][x].g > maxPixelValue)
              {
                cout << "ERROR: invalid color value " << image[y][x].g << endl;
                return false;
              }

              inputFile >> image[y][x].b;

              if (image[y][x].b == -1)
              {
                if (inputFile.eof())
                {
                  cout << "ERROR: not enough color values 3" << endl;
                  return false;
                }
                cout << "ERROR: read non-integer value" << endl;
                return false;
              }
              if (image[y][x].b < 0 || image[y][x].b > maxPixelValue)
              {
                cout << "ERROR: invalid color value " << image[y][x].b << endl;
                return false;
              }
            }
          }
        }
        else
        {
          //input height and width do not match the height and width in the file
          //standard out messages are in the function inputValidation
          return false;
        }
      }
      else
      {
        //bad data
        cout << "ERROR: read non-integer value" << endl;
        return false;
      }
    }
    else
    {
      //the file type is not correct
      cout << "ERROR: type is " << fileType << " instead of P3" << endl;
      return false;
    }
    //check if there is any leftover data before closing file
    int tmp = 0;
    inputFile >> tmp;
    if (tmp != 0)
    {
      cout << "ERROR: too many color values" << endl;
    }

    inputFile.close();
  }
  else
  {
    cout << "ERROR: failed to open input file - " << filename << endl;
    return false;
  }

  cout << " ... Completed Loading image ... " << endl;
  return true;
}

// function to generate an output image
bool outputImage(string filename, Pixel **image, int width, int height)
{
  cout << " Outputing image ... with filename  " << filename << endl;
  std::string fileType;

  //takes filename and reads it line by line
  //line 2 has information about the width  = number of columns and height  = number of rows
  ofstream outputFile(filename);
  if (outputFile.is_open())
  {
    cout << "made an output file" << endl;

    outputFile << "P3" << endl;
    outputFile << width << " " << height << endl;
    outputFile << "255" << endl;
    for (int x = 0; x < height; ++x)
    {
      for (int y = 0; y < width; ++y)
      {
        outputFile << image[y][x].r;
        outputFile << " ";
        outputFile << image[y][x].g;
        outputFile << " ";
        outputFile << image[y][x].b;
        outputFile << " ";
      }
    }
    outputFile.close();
  }
  else
  {

    cout << " Couldnt open output file " << endl;
    return false;
  }

  cout << " Completed Loading image ... " << endl;
  return true;
}

//function to calculate energy of an image
int energy(Pixel **image, int x, int y, int width, int height)
{
  int xUp, xDown, yUp, yDown;
  //border case
  if (x == 0)
  {
    xUp = 1;
    xDown = width - 1;
  }
  if (x == width - 1)
  {
    xUp = 0;
    xDown = x - 1;
  }
  if (y == 0)
  {

    yUp = 1;
    yDown = height - 1;
  }
  if (y == height - 1)
  {
    yUp = 0;
    yDown = y - 1;
  }
  if (x != 0 && x != width - 1)
  {
    xUp = x + 1;
    xDown = x - 1;
  }
  if (y != 0 && y != height - 1)
  {
    yUp = y + 1;
    yDown = y - 1;
  }

  int sqXGradient, sqYGradient, centralDiffxRed, centralDiffxGreen, centralDiffxBlue, centralDiffyRed, centralDiffyGreen, centralDiffyBlue, energyScore;
  centralDiffxRed = pow(abs(image[xUp][y].r - image[xDown][y].r), 2);
  centralDiffxBlue = pow(abs(image[xUp][y].b - image[xDown][y].b), 2);
  centralDiffxGreen = pow(abs(image[xUp][y].g - image[xDown][y].g), 2);
  centralDiffyRed = pow(abs(image[x][yUp].r - image[x][yDown].r), 2);
  centralDiffyBlue = pow(abs(image[x][yUp].b - image[x][yDown].b), 2);
  centralDiffyGreen = pow(abs(image[x][yUp].g - image[x][yDown].g), 2);
  sqXGradient = (centralDiffxRed + centralDiffxGreen + centralDiffxBlue);
  sqYGradient = (centralDiffyRed + centralDiffyGreen + centralDiffyBlue);
  energyScore = sqXGradient + sqYGradient;
  return energyScore;
}

// function to calculate vertical seams
int loadVerticalSeam(Pixel **image, int start_col, int width, int height, int *seam)
{

  seam[0] = start_col; // set the firt row at start column
  int totalEnergy = energy(image, start_col, 0, width, height);
  int minEnergy = totalEnergy;
  int a, b, c; //placeholders for min energy
  //loop through all rows and add seam one at a time
  int x = start_col;
  for (int i = 1; i < height; i++)
  {
    //if at the edges then two options
    if (x == width - 1)
    {
      a = energy(image, x, i, width, height);
      b = energy(image, x - 1, i, width, height);
      //check which is lower energy
      if (a < b)
      {

        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b < a)
      {
        minEnergy = b;
        seam[i] = x - 1;
        x = seam[i];
      }
      else if (a == b)
      {
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
    }
    else if (x == 0)
    {
      a = energy(image, x, i, width, height);
      b = energy(image, x + 1, i, width, height);
      //check which is lower energy
      if (a < b)
      {
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b < a)
      {
        minEnergy = b;
        seam[i] = x + 1;
        x = seam[i];
      }
      else if (a == b)
      {
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
    }

    else
    {
      a = energy(image, x, i, width, height);
      b = energy(image, x + 1, i, width, height);
      c = energy(image, x - 1, i, width, height);
      //check which is lower energy
      if (a < b && a < c)
      {
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b < a && b < c)
      {
        minEnergy = b;
        seam[i] = x + 1;
        x = seam[i];
      }
      else if (c < a && c < b)
      {
        minEnergy = c;
        seam[i] = x - 1;
        x = seam[i];
      }
      else if (a == c && a < b)
      {

        //go down
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (a == b && a < c)
      {

        //go down
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b == c && b < a)
      {
        //go to b
        minEnergy = b;
        seam[i] = x + 1;
        x = seam[i];
      }
      else if (a == b && b == c)
      {
        //go down
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
    }

    totalEnergy = totalEnergy + minEnergy;
  }

  return totalEnergy;
}

// function to calculate horizontal seams
int loadHorizontalSeam(Pixel **image, int start_row, int width, int height, int *seam)
{
  seam[0] = start_row; // set the firt row at start column
  int totalEnergy = energy(image, 0, start_row, width, height);
  int minEnergy = totalEnergy;
  int a, b, c; //placeholders for min energy
  //loop through all rows and add seam one at a time

  int x = start_row;
  for (int i = 1; i < width; i++)
  {
    //if at the edges then two options
    if (x == height - 1)
    {
      a = energy(image, i, x, width, height);
      b = energy(image, i, x - 1, width, height);
      //check which is lower energy
      if (a < b)
      {
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b < a)
      {
        minEnergy = b;
        seam[i] = x - 1;
        x = seam[i];
      }
      else if (a == b)
      {
        minEnergy = a;
        seam[i] = x;
      }
    }
    else if (x == 0)
    {
      a = energy(image, i, x, width, height);
      b = energy(image, i, x + 1, width, height);
      //check which is lower energy
      if (a < b)
      {
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b < a)
      {
        minEnergy = b;
        seam[i] = x + 1;
        x = seam[i];
      }
      else if (a == b)
      {
        minEnergy = a;
        seam[i] = x;
      }
    }

    else
    {
      a = energy(image, i, x, width, height);
      b = energy(image, i, x + 1, width, height);
      c = energy(image, i, x - 1, width, height);
      //check which is lower energy
      if (a < b && a < c)
      {
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b < a && b < c)
      {
        minEnergy = b;
        seam[i] = x + 1;
        x = seam[i];
      }
      else if (c < a && c < b)
      {
        minEnergy = c;
        seam[i] = x - 1;
        x = seam[i];
      }
      else if (a == c && a < b)
      {
        //go down
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (a == b && a < c)
      {
        //go down
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
      else if (b == c && b < a)
      {
        //go to c
        minEnergy = c;
        seam[i] = x - 1;
        x = seam[i];
      }
      else if (a == b && b == c)
      {
        //go down
        minEnergy = a;
        seam[i] = x;
        x = seam[i];
      }
    }

    totalEnergy = totalEnergy + minEnergy;
  }

  return totalEnergy;
}

// function to find vertical seam with the minimum energy
int *findMinVerticalSeam(Pixel **image, int width, int height)
{
  {
    int minEnergy;
    int temp;
    int *Seam = createSeam(height);
    int *verticalSeam = new int[height];
    minEnergy = loadVerticalSeam(image, 0, width, height, Seam);
    for (int j = 0; j < height; j++)
    {
      verticalSeam[j] = Seam[j];
    }
    for (int i = 1; i < width; i++)
    {

      temp = loadVerticalSeam(image, i, width, height, Seam);

      if (temp < minEnergy)
      {
        minEnergy = temp;
        for (int k = 0; k < height; k++)
        {
          verticalSeam[k] = Seam[k];
        }
      }
    }
    deleteSeam(Seam);
    return verticalSeam;
  }
}

// function to find horizontal seam with the minimum energy
int *findMinHorizontalSeam(Pixel **image, int width, int height)
{
  int minEnergy;
  int temp;
  int *Seam = createSeam(width);
  int *horizontalSeam = new int[width];
  minEnergy = loadHorizontalSeam(image, 0, width, height, Seam);
  for (int j = 0; j < width; j++)
  {
    horizontalSeam[j] = Seam[j];
  }
  for (int i = 1; i < height; i++)
  {

    temp = loadHorizontalSeam(image, i, width, height, Seam);

    if (temp < minEnergy)
    {
      minEnergy = temp;
      for (int k = 0; k < width; k++)
      {
        horizontalSeam[k] = Seam[k];
      }
    }
  }
  deleteSeam(Seam);
  return horizontalSeam;
}

// function to remove a given vertical seam from the image
void removeVerticalSeam(Pixel **image, int width, int height, int *verticalSeam)
{
  for (int i = 0; i < height; i++)
  {
    for (int j = verticalSeam[i]; j < width - 1; j++)
    {
      image[j][i] = image[j + 1][i];
    }
  }
}

//function to remove a given horizontal seam from the image
void removeHorizontalSeam(Pixel **image, int width, int height, int *horizontalSeam)
{
  for (int i = 0; i < width; i++)
  {
    for (int j = horizontalSeam[i]; j < height - 1; j++)
    {
      image[i][j] = image[i][j + 1];
    }
  }
}
