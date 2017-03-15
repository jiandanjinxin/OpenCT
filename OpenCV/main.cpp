//dcm images processing main function
extern int opencv_main(int argc, char* argv[]);

//mnist dataset using easycnn framework main function
extern int mnist_main(int argc, char* argv[]);

//cuda test
extern int cuda_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	//return opencv_main(argc, argv);
	return mnist_main(argc, argv);
}