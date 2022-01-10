#include <stdbool.h> 
// DEFINE CONSTS THAT USED AT 'applypixel' and used in the file
#define KERNEL_SIZE 3
#define MIN_INTENSITY 766
#define MAX_INTENSITY -1
// define function here make the access faster
#define MIN(a,b) ((a<b ? a:b))
#define MAX(a,b) ((a>b ? a:b))
#define CALC_INDEX(i,j,n) ((i)*(j) + (j))

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;


/* Compute min and max of two integers, respectively */
int min(int a, int b) { return (a < b ? a : b); }
int max(int a, int b) { return (a > b ? a : b); }

int calcIndex(int i, int j, int n) {
	return ((i)*(n)+(j));
}

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
	current_pixel->red = (unsigned char) (min(max(sum.red, 0), 255));
	current_pixel->green = (unsigned char) (min(max(sum.green, 0), 255));
	current_pixel->blue = (unsigned char) (min(max(sum.blue, 0), 255));
	return;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
	sum->red += ((int) p.red) * weight;
	sum->green += ((int) p.green) * weight;
	sum->blue += ((int) p.blue) * weight;
	// sum->num++;
	return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int ii, jj;
	int currRow, currCol;
	pixel_sum sum;
	pixel current_pixel;
	int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
	int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
	int min_row, min_col, max_row, max_col;
	pixel loop_pixel;

	initialize_pixel_sum(&sum);

	for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
		for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {

			int kRow, kCol;

			// compute row index in kernel
			if (ii < i) {
				kRow = 0;
			} else if (ii > i) {
				kRow = 2;
			} else {
				kRow = 1;
			}

			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			// apply kernel on pixel at [ii,jj]
			sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
		}
	}

	if (filter) {
		// find min and max coordinates
		for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
			for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
				// check if smaller than min or higher than max and update
				loop_pixel = src[calcIndex(ii, jj, dim)];
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
					min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					min_row = ii;
					min_col = jj;
				}
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
					max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					max_row = ii;
					max_col = jj;
				}
			}
		}
		// filter out min and max
		sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
		sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
	}

	// assign kernel's result to pixel at [i,j]
	assign_sum_to_pixel(&current_pixel, sum, kernelScale);
	return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int i, j;
	for (i = kernelSize / 2 ; i < dim - kernelSize / 2; i++) {
		for (j =  kernelSize / 2 ; j < dim - kernelSize / 2 ; j++) {
			dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);
		}
	}
}

//got replaced by memcpy
void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			pixels[row*n + col].red = image->data[3*row*n + 3*col];
			pixels[row*n + col].green = image->data[3*row*n + 3*col + 1];
			pixels[row*n + col].blue = image->data[3*row*n + 3*col + 2];
		}
	}
}


void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			image->data[3*row*n + 3*col] = pixels[row*n + col].red;
			image->data[3*row*n + 3*col + 1] = pixels[row*n + col].green;
			image->data[3*row*n + 3*col + 2] = pixels[row*n + col].blue;
		}
	}
}

//got replaced by memcpy
void copyPixels(pixel* src, pixel* dst) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			dst[row*n + col].red = src[row*n + col].red;
			dst[row*n + col].green = src[row*n + col].green;
			dst[row*n + col].blue = src[row*n + col].blue;
		}
	}
}

void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

    // instead multiplying n*m*size of we'll do one calculation
    int size = m*n*sizeof(pixel);
	pixel* pixelsImg = malloc(size);
	pixel* backupOrg = malloc(size);

	//instead of calling a function, we can use memcpy:
	
	memcpy(pixelsImg, image->data, size);//copyng 'size' byts from image->data to pixelsImg
	//charsToPixels(image, pixelsImg);
	memcpy(backupOrg, pixelsImg,size);
	//copyPixels(pixelsImg, backupOrg);

	smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}

// Writting all the function at this scope make the code faster
void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {
	// instead multiplying n*m*size of we'll do one calculation
    int size = m*n*sizeof(pixel);
	pixel* pixelsImg = malloc(size);
	pixel* backupOrg = malloc(size);

	// Register members can be more accessiables
	register int i,j; //members for conditions loop
	register int m_kernel = m-1; // right corner border
	register pixel p;
	register pixel current_pixel;
	register int red_sum, green_sum, blue_sum;
	register int in_place; // integer for current index

	if (flag == '1') {	
	//instead of calling a function, we can use memcpy:
	memcpy(pixelsImg, image->data, size);//copyng 'size' byts from image->data to pixelsImg
	//charsToPixels(image, pixelsImg);
	memcpy(backupOrg, pixelsImg,size);
	//copyPixels(pixelsImg, backupOrg);
	i = m_kernel;
	/**
	 * loop stop when i==0
	 * while loop more efficient then for
	 * this is represent the 'doConvolution' func
	**/
	
	/**
	#blur image:
	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	//int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	/** to create matrix and insert in function may take time so 
	 * all the calculation will be imidiatlly.
	**/
	while(--i){
		j=m_kernel;
		while (--j){
			/**
			 * this part is like smooth inside convolution
			 * and the 'applyKernel' func
			**/
			red_sum = green_sum = blue_sum = 0;
			in_place = (i-1)*m;
			in_place += j-1;
			// applyKernel at in_place:
			// backup0rg[0,0]
			p = backupOrg[in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[0,1]
			p = backupOrg[++in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[0,2]
			p = backupOrg[++in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[1,0]
			in_place += (m-2);
			p = backupOrg[in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[1,1]
			p = backupOrg[++in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[1,2]
			p = backupOrg[++in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[2,0]
			in_place += (m-2);			
			p = backupOrg[in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[2,1]
			p = backupOrg[++in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// backup0rg[2,2]
			p = backupOrg[++in_place];
			red_sum += p.red;
			green_sum += p.green;
			blue_sum += p.blue;
			// now assign result pixel at [i][j]
			// and find if there's max/min
			current_pixel.red = (unsigned char)(MIN(MAX(red_sum/9,0),255));
			current_pixel.blue = (unsigned char)(MIN(MAX(blue_sum/9,0),255));
			current_pixel.green = (unsigned char)(MIN(MAX(green_sum/9,0),255));
			pixelsImg[CALC_INDEX(i,j,m)] = current_pixel;
		}
	}
	//pixelToChar(backupOrg,pixelsImg);
	memcpy(image->data,pixelsImg,size);
	// write result image to file
	writeBMP(image, srcImgpName, blurRsltImgName);	
	//doConvolution(image, 3, blurKernel, 9, false);
	//**********//
	/**
	#Sharp image:
	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	//int sharpKernel[3][3] = {{-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1}};
	/** to create matrix and insert in function may take time so 
	 * all the calculation will be imidiatlly.
	**/
	//same as blur:
	memcpy(pixelsImg, image->data, size);
	memcpy(backupOrg, pixelsImg,size);
	i = m_kernel;
	while(--i){
		j=m_kernel;
		while (--j){
			// now the initialation is -1 for sharpen
			red_sum = green_sum = blue_sum = 0;
			in_place = (i-1)*m;
			in_place += j-1;
			// applyKernel at in_place:
			// backup0rg[0,0]
			p = backupOrg[in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// backup0rg[0,1]
			p = backupOrg[++in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// backup0rg[0,2]
			p = backupOrg[++in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// backup0rg[1,0]
			in_place += (m-2);
			p = backupOrg[in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// backup0rg[1,1] for current pixel cell
			p = backupOrg[++in_place];
			red_sum += p.red * 9;
			green_sum += p.green* 9;
			blue_sum += p.blue * 9;
			// backup0rg[1,2]
			p = backupOrg[++in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// backup0rg[2,0]
			in_place += (m-2);			
			p = backupOrg[in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// backup0rg[2,1]
			p = backupOrg[++in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// backup0rg[2,2]
			p = backupOrg[++in_place];
			red_sum -= p.red;
			green_sum -= p.green;
			blue_sum -= p.blue;
			// now assign result pixel at [i][j]
			// and find if there's max/min
			current_pixel.red = (unsigned char)(MIN(MAX(red_sum,0),255));
			current_pixel.blue = (unsigned char)(MIN(MAX(blue_sum,0),255));
			current_pixel.green = (unsigned char)(MIN(MAX(green_sum,0),255));
			pixelsImg[CALC_INDEX(i,j,m)] = current_pixel;
		}
	}
	//pixelToChar(backupOrg,pixelsImg);
	memcpy(image->data,pixelsImg,size);
	// write result image to file
	writeBMP(image, srcImgpName, sharpRsltImgName);	
	//doConvolution(image, 3, sharpKernel, 1, false);	

	} else {
		// apply extermum filtered kernel to blur image
		doConvolution(image, 3, blurKernel, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, 3, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

