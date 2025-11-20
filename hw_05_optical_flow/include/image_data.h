#ifndef image_data_H
#define image_data_H


#include <memory>
#include <vector>
#include <stdexcept>
#include <OpenImageIO/imageio.h>

#include "string_funcs.h"


class ImageData {
public:
	ImageData();
	ImageData(const char* filename) { oiio_read(filename); }
	ImageData(const ImageData& other);
	ImageData(const int width, const int height, const int channels);
	~ImageData();

	void set_to(const ImageData& other);

	void oiio_read(const char* filename);
	void oiio_write();
	void oiio_write(const std::string& file_extension);
	
	void set_dimensions(int width, int height, int channels);	
	
	const float* get_pixel_ptr() const { return _image_data_ptr.get(); }
	float* get_pixel_ptr() { return _image_data_ptr.get(); }
	
	const std::vector<float> get_pixel_values(int x, int y) const;
	std::vector<float> get_pixel_values(int x, int y);
	void get_pixel_values(int x, int y, std::vector<float>& values) const;
	const float& get_pixel_value(int x, int y, int c) const { return _image_data_ptr[(y * _width + x) * _channels + c]; }

	void set_pixel_value(const int x, const int y, const int c, const float val);
	void set_pixel_values(int x, int y, const std::vector<float>& values);
	void set_pixel_values(const std::vector<float>& values);
	void set_pixel_values(const float val);
	void set_first_three_channels(int x, int y, const std::vector<float>& values);
	void set_first_three_channels(int x, int y, float r, float g, float b);


	void add_values(int x, int y, float r, float g, float b, float a);
	void add_value(int x, int y, int channel, float val);
	void add_pixels_into(int x, int y, std::vector<float>& pixel_values);
	void divide_each_pixel_by(const std::vector<float>& vals);
	
	void mix_rgb_values(int x, int y, float r, float g, float b);
	void mix_rgb_values(int x, int y, float r, float g, float b, float weight);

	void scale_pixel_values(const int x, const int y, const float scale_factor);
	void scale_values(float scale_factor);

	long get_index(int x, int y, int channel) const;
	long get_index(int x, int y) const;
	
	const int& get_width() const { return _width; }
	const int& get_height() const { return _height; }
	const int& get_channels() const { return _channels; }
	int get_data_len() const;
	
	std::vector<float> get_max() const;
	std::vector<float> get_min() const;

	std::vector<float> get_average() const;
	std::vector<float> get_rms() const;
	std::vector<float> get_rms(const std::vector<float>& avg) const;

	std::vector<float> interpolate_nearest_neighbor(const float x, const float y) const;
	std::vector<float> interpolate_bilinear(const float x, const float y) const;
	float interpolate_bilinear(const float x, const float y, const int c) const;

	ImageData get_x_y_gradients() const;
	ImageData get_gradient(const bool is_x_gradient) const;

	void clear();
	
	std::string get_output_file_name() const { return _file_name + "_out." + _file_type; }
	
	ImageData duplicate() const;

	ImageData& operator=(const ImageData& other);
	ImageData& operator*=(const ImageData& other);
	ImageData& subtract_then_multiply(const ImageData& sub_img, const ImageData& mul_img);
	
	

protected:
	int _width;
	int _height;
	int _channels;
	
	std::unique_ptr<float[]> _image_data_ptr;

private:
	std::string _file_type;
	std::string _file_name;
};


#endif