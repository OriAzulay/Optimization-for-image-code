// Ori Azulay, ID: 206336794
// DEFINE CONSTS THAT USED AT 'applypixel' and used in the file
#define KERNEL_SIZE 3
#define MIN_INTENSITY 766
#define MAX_INTENSITY -1
// define function here make the access faster
#define MIN(a,b) ((a<b ? a:b))
#define MAX(a,b) ((a>b ? a:b))
#define CALC_INDEX(i,j,n) ((i)*(n) + (j))
// dont need include stdbool anymore
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

// Writting all the function at this scope make the code faster
void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {
	// instead multiplying n*m*size of we'll do one calculation
    int size = m*n*sizeof(pixel);
	pixel* pixelsImg = malloc(size);
	pixel* backupOrg = malloc(size);

	// Register members can be more accessiables
	 int i,j; //members for conditions loop
	 int m_kernel = m-1; // right corner border
	 pixel p;
	 pixel current_pixel;
	 int red_sum, green_sum, blue_sum;
	 int in_place; // integer for current index

	if (flag == '1') {	
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
	/** to create matrix and insert in function may take time so 
	 * all the calculation will be imidiatlly.
	**/
	//instead of calling a function, we can use memcpy:
	memcpy(pixelsImg, image->data, size);//copyng 'size' byts from image->data to pixelsImg
	//charsToPixels(image, pixelsImg);
	memcpy(backupOrg, pixelsImg,size);
	//copyPixels(pixelsImg, backupOrg);
	i = m_kernel;
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
		// filter is on therfire the if condition at applyKernel is on:
		memcpy(pixelsImg, image->data, size);//copyng 'size' byts from image->data to pixelsImg
		//charsToPixels(image, pixelsImg);
		memcpy(backupOrg, pixelsImg,size);
		//copyPixels(pixelsImg, backupOrg);
		int min_row, min_col, max_row, max_col;
		register int RedGreenBluePixel; //sum_pixels_by_weight - Sums pixel values, scaled by given weight
		i = m_kernel;
		while(--i){
			j=m_kernel;
			while(--j){
				//applyKernal part:
				int min_intens = MIN_INTENSITY; // tempurary for eqaulity
				int max_intens = MAX_INTENSITY;
				red_sum = green_sum = blue_sum = 0;
				in_place = (i-1) * m ;
				in_place += j-1 ;
				// backup0rg[0,0]
				p = backupOrg[in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				//claculate outside the loop
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i-1;
					min_col = j-1;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i-1;
					max_col = j-1;
				}
				// backup0rg[0,1]
				p = backupOrg[++in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i-1;
					min_col = j;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i-1;
					max_col = j;
				}
				// backup0rg[0,2]
				p = backupOrg[++in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i-1;
					min_col = j+1;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i-1;
					max_col = j+1;
				}
				// backup0rg[1,0]
				in_place += (m-2);
				p = backupOrg[in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i;
					min_col = j-1;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i;
					max_col = j-1;
				}
				// backup0rg[1,1]
				p = backupOrg[++in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i;
					min_col = j;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i;
					max_col = j;
				}
				
				// backup0rg[1,2]
				p = backupOrg[++in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i;
					min_col = j+1;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i;
					max_col = j+1;
				}
				// backup0rg[2,0]
				in_place += (m-2);
				p = backupOrg[in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i+1;
					min_col = j-1;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i+1;
					max_col = j-1;
				}
				// backup0rg[2,1]
				p = backupOrg[++in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = (((int)p.red)+ ((int)p.blue) + ((int)p.green));
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i+1;
					min_col = j;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i+1;
					max_col = j;
				}
				// backup0rg[2,2]
				p = backupOrg[++in_place];
				red_sum += p.red;
				green_sum += p.green;
				blue_sum += p.blue;
				RedGreenBluePixel = ( ((int)p.red) + ((int)p.blue) + ((int)p.green) );
				// check if smaller than min or higher than max and update
				if(RedGreenBluePixel <= min_intens){
					min_intens = RedGreenBluePixel;
					min_row = i+1;
					min_col = j+1;
				}
				if(RedGreenBluePixel > max_intens){
					max_intens = RedGreenBluePixel;
					max_row = i+1;
					max_col = j+1;
				}

				//filter max & min
				pixel calcIndex = backupOrg[CALC_INDEX(min_row,min_col,m)];
				red_sum -= ((int)calcIndex.red);	
				green_sum -= ((int)calcIndex.green);
				blue_sum -= ((int)calcIndex.blue);	
				calcIndex = backupOrg[CALC_INDEX(max_row,max_col,m)];
				red_sum -= ((int)calcIndex.red);	
				green_sum -= ((int)calcIndex.green);
				blue_sum -= ((int)calcIndex.blue);		

				// pixel's color value in range [0,255]
				current_pixel.red = (unsigned char)(MIN(MAX(red_sum/7,0),255));
				current_pixel.blue = (unsigned char)(MIN(MAX(blue_sum/7,0),255));
				current_pixel.green = (unsigned char)(MIN(MAX(green_sum/7,0),255));
				pixelsImg[CALC_INDEX(i,j,m)] = current_pixel;
			}
		}
		memcpy(image->data, pixelsImg,size);
		//doConvolution(image, 3, blurKernel, 7, true);
		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		//same as before:
		//#Sharp image:
		/*
		* [-1, -1, -1]
		* [-1, 9, -1]
		* [-1, -1, -1]
		*/
		//int sharpKernel[3][3] = {{-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1}};
		/** to create matrix and insert in function may take time so 
		 * all the calculation will be imidiatlly.
		**/
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
				current_pixel.green = (unsigned char)(MIN(MAX(green_sum,0),255));
				current_pixel.blue = (unsigned char)(MIN(MAX(blue_sum,0),255));
				pixelsImg[CALC_INDEX(i,j,m)] = current_pixel;
			}
		}
		//doConvolution(image, 3, sharpKernel, 1, false);
		memcpy(image->data, pixelsImg,size);
		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
	free(pixelsImg);
	free(backupOrg);
}

