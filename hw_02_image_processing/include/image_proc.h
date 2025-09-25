#ifndef IMAGE_PROC_H
#define IMAGE_PROC_H


#include <memory>
#include <vector>
#include <stdexcept>
#include <OpenImageIO/imageio.h>

#include "string_funcs.h"


class ImageProc {
public:
	ImageProc();
	ImageProc(const char* filename) { oiio_read(filename); }
	ImageProc(const ImageProc& other);
	~ImageProc();

	void oiio_read(const char* filename);
	void oiio_write();
	
	void set_dimensions(int width, int height, int channels);	
	int get_data_len() const;
	
	const std::vector<float> get_pixel_values(int x, int y) const;
	std::vector<float> get_pixel_values(int x, int y);

	
	const float* get_pixel_ptr() const { return _image_data_ptr.get(); }
	float* get_pixel_ptr() { return _image_data_ptr.get(); }
	
	void set_pixel_values(int x, int y, const std::vector<float>& values);
	void set_pixel_values(const std::vector<float>& values);
	
	long get_index(int x, int y, int channel) const;
	long get_index(int x, int y) const;
	
	const int& get_width() const { return _width; }
	const int& get_height() const { return _height; }
	const int& get_channels() const { return _channels; }
	
	void clear();
	
	std::string get_output_file_name() const { return _file_name + "_out." + _file_type; }
	
	ImageProc& operator=(const ImageProc& other);

private:
	int _width;
	int _height;
	int _channels;
	bool _is_flipped = false;
	std::string _file_type;
	std::string _file_name;

	std::unique_ptr<float[]> _image_data_ptr;


	void _vertical_flip();
};


#endif