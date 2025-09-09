#ifndef IMAGE_PROC_H
#define IMAGE_PROC_H


#include <memory>
#include <vector>
#include <stdexcept>

#include <OpenImageIO/imageio.h>

class ImageProc {
public:
	ImageProc();
	~ImageProc();

	void set_dimensions(int width, int height, int channels);	
	int get_data_len() const;

	const std::vector<float> get_pixel_values(int x, int y) const;
	
	std::vector<float> get_pixel_values(int x, int y);
	void get_pixel_values(int x, int y, std::vector<float>& values) const;

	void set_pixel_values(int x, int y, const std::vector<float>& values);
	void set_pixel_values(const std::vector<float>& values);

	long get_index(int x, int y, int channel) const;
	long get_index(int x, int y) const;

	const int& get_width() const { return _width; }
	const int& get_height() const { return _height; }
	const int& get_channels() const { return _channels; }

	void oiio_read(const char* filename);
	void oiio_write(const char* filename) const;

	// TO DO -> Deep copy methods. Also deep copy assignment 
	ImageProc(const ImageProc& other);
	ImageProc& operator=(const ImageProc& other);


private:
	int _width;
	int _height;
	int _channels;

	std::unique_ptr<float[]> _image_data_ptr;

};


#endif