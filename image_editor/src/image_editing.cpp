#include "image_editing.h"

ImageEditor::~ImageEditor() {}


void ImageEditor::save_edited_image() {
	if (_edited_image){
		_edited_image->oiio_write();
	}
}

void ImageEditor::save_edited_image(const std::string& file_extension){
	if (_edited_image){
		_edited_image->oiio_write(file_extension);
	}
}

ImageData ImageEditor::optical_flow(
	const std::vector<std::string>& image_sequence,
	const ImageData& img_to_flow,
	std::string output_dir,
	int iterations_per_image
){
	
	std::vector<ImageData> flow_sequence;
	flow_sequence.reserve(image_sequence.size());
	for (const auto& filename : image_sequence) {
		ImageData img(filename.c_str());
		flow_sequence.push_back(img);
	}
	std::vector<size_t> indices;
	for (size_t i = 0; i < flow_sequence.size(); i++) {
		indices.push_back(i);
	}
	OpticalFlow flow_driver(flow_sequence, img_to_flow);
	return flow_driver.flow(indices, output_dir, iterations_per_image);
}

void ImageEditor::optical_flow_video(
	const std::vector<std::string>& video_frame_sequence,
	int flow_frames_per_image,
	std::string output_dir,
	int iterations_per_image,
	bool reverse_sequence,
	bool reverse_flow_direction
){
	
	const ImageData first_img(video_frame_sequence[0].c_str());
	std::vector<ImageData> video_imgs;
	video_imgs.reserve(video_frame_sequence.size());
	for (const auto& filename : video_frame_sequence) {
		ImageData img(filename.c_str());
		video_imgs.push_back(img);
	}

	OpticalFlow flow_driver(video_imgs, first_img);
	
	// Actually performing the flow
	if (reverse_sequence){
		for (size_t i = 0; i < video_frame_sequence.size(); i++){
			flow_driver.set_new_target_image(video_imgs[i]);
			
			std::vector<size_t> flow_sequence;
			size_t flow_frames = std::min(static_cast<size_t>(flow_frames_per_image), i);
			for (int f = 0; f < flow_frames; f++){
				// Try to get the reverse flow. So flow this based on past frames
				flow_sequence.push_back(i - f - 1);
			}
			if (flow_sequence.empty()){
				continue;
			}
			ImageData flowed_image = flow_driver.flow(flow_sequence, "", iterations_per_image);
				
			std::string img_n = StringFuncs::get_zero_padded_number_string(i, 4);
			std::string out_filename = output_dir + "/flowed_" + img_n + "." + flowed_image.get_ext();
			//ImageData blended_image = video_imgs[i].duplicate();
			//blend_images(blended_image, flowed_image, 0.8f);
			// Blend the orignal image with the flowed image
			//blended_image.oiio_write_to(out_filename);
			flowed_image.oiio_write_to(out_filename);
			
			std::cout << "Completed optical flow for frame " << i << " of " << video_frame_sequence.size() << std::endl;
		} // End loop over video frames
	} // End if reverse sequence
	else {
		for (size_t i = 0; i < video_frame_sequence.size() - 1; i++){
			flow_driver.set_new_target_image(video_imgs[i]);
			
			std::vector<size_t> flow_sequence;
			size_t flow_frames = std::min(static_cast<size_t>(flow_frames_per_image), video_frame_sequence.size() - i - 1);
			for (int f = 0; f < flow_frames; f++){
				flow_sequence.push_back(i + f + 1);
			}
			ImageData flowed_image = flow_driver.flow(flow_sequence, "", iterations_per_image);
			std::string img_n = StringFuncs::get_zero_padded_number_string(i, 4);
			std::string out_filename = output_dir + "/flowed_" + img_n + "." + flowed_image.get_ext();
			
			// ImageData blended_image = video_imgs[i].duplicate();
			// blend_images(blended_image, flowed_image, 0.8f);
			// blended_image.oiio_write_to(out_filename);
			flowed_image.oiio_write_to(out_filename);
			std::cout << "Completed optical flow for frame " << i << " of " << video_frame_sequence.size() << std::endl;
		}
	}
}