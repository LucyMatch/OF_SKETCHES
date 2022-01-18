/*
	v 1 - Local Media Manager
	Lucy Matchett - 2022

	Handles Local Video + Image files
	A "feed" is created for each desired media source
	This allows the handler to manage multiple videos / image sets etc at once
	creating a feed returns a pointer to the feed obj
	use this pointer to request new frames / retrieve media texture

*/
#pragma once

#include "VideoHandler.h"
#include "ImageHandler.h"

enum mediaTypes {
	/// \Single Image
	IMAGE = 0,
	/// \Array of Images
	IMAGE_COLLECTION = 1,
	/// \Video Feed
	VIDEO = 2
};

struct Feed {
	int id = 0;										/// \ID & index
	mediaTypes media_type;							/// \Media type
	string path;									/// \Path to media directory
	bool enable_slideshow = false;					/// \If true image file updated to next in directory based on freq / is false static image
	int slideshow_frequency = 60;					/// \Frames between Image updates
	bool isFrameNew = false;						/// \True if media has been updated
	VideoHandler* vids = NULL;						/// \Pointer to videohandler obj if applicable
	ImageHandler* imgs = NULL;						/// \Pointer to imageHandler obj if applicable 
	int update_counter = 0;							/// \Running counter of updates - for slideshow frequency
	int curr_image = 0;								/// \Current Image Index
	int collection_size = 1;						/// \How many images are in a collection
	bool active = false;							/// \Is this feed being used? or open for use?
};

class LocalMediaManager {

public:

	LocalMediaManager() {};

	Feed* createNewFeed(const Feed& new_feed) {

		bool created = false;

		//find first availble feed
		//we have a fixed amount array
		for (auto &f : feeds) {

			if (!checkFeed(f)) {
				//feed not being used!

				f = new_feed;

				//determine media type
				if (f.media_type < 2) {
					////image based types
					images.push_back(new ImageHandler(f.path));
					f.imgs = images[images.size() - 1];
					created = true;
				}
				else if (f.media_type == 2) {
					//video based types
					VideoHandler* vh = new VideoHandler(glm::vec2(1920, 1080));
					vh->setup(f.path, VideoHandler::VIDEO_LOCAL);
					vh->setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
					videos.push_back(vh);
					f.vids = videos[videos.size()-1];
					created = true;
				}

				if (created) {
					//set the feed to active!
					f.active = true;
					return &f;	//check what we are returning here is correct....
				}

			}
		}

		return NULL;

	}
	
	void update() {

		for (auto v : videos)
			v->update();

		for ( auto &f : feeds) {

			//if its an active feed do the things
			if (checkFeed(f)) {
				//check frames
				if (f.media_type < 2) {
					//image types
					if (f.enable_slideshow) {
						if (f.update_counter % f.slideshow_frequency == 0) {
							f.isFrameNew = true;
							nxtImage(&f);
						}
					}
				}
				else if (f.media_type == 2) {
					//video types
					f.isFrameNew = f.vids->isFrameNew();
				}
			}
		}
	}

	void drawDebug() {

		int amt = sizeof(feeds);
		int cols = 3;
		int rows = glm::ceil(amt / cols);
		glm::vec2 pos(0, 0);
		int counter = 0;

		for (auto &f : feeds) {

			//if its an active feed do the things
			if (checkFeed(f)) {

				if (counter % cols != 0) pos.x = ofGetWidth() / cols;
				pos.y = glm::floor((counter / cols)) * (ofGetHeight() / 2);

				switch (f.media_type) {
				case VIDEO:
					if (checkVideoHandler(*f.vids))
						f.vids->getFrameTex()->draw(pos.x, pos.y);
					break;
				case IMAGE :
					if (checkImageHandler( *f.imgs )) 
						f.imgs->getImages()[f.curr_image].draw(pos.x, pos.y);
					break;
				case IMAGE_COLLECTION :
					if (checkImageHandler( *f.imgs )) {
						int cols = 2;
						int rows = glm::ceil(f.collection_size / cols);
						glm::vec2 nested_pos(0, 0);
						for (int i = 0; i < f.collection_size; i++) {
							ofPushMatrix();
							ofPushStyle();

							ofTranslate(nested_pos.x, nested_pos.y);
							ofScale(0.25);
							f.imgs->getImages()[f.curr_image].draw(pos.x, pos.y);
							nxtImage(&f);

							ofPopStyle();
							ofPopMatrix();
						}
					}
					break;
				}
				counter++;
			}
		}
	}

	void nxtImage(Feed* _feed) {
		cout << "feed " << _feed->id << " curr img index = " << _feed->curr_image << endl;
		(*_feed).curr_image = ++(*_feed).curr_image % _feed->imgs->getImages().size();
		cout << "feed " << _feed->id << " updated curr img index = " << _feed->curr_image << endl;
	}

	void prevImage(Feed* _feed) {
		cout << "feed " << _feed->id << " curr img index = " << _feed->curr_image << endl;
		(*_feed).curr_image = --(*_feed).curr_image % _feed->imgs->getImages().size();
		if (_feed->curr_image < 0)(*_feed).curr_image = _feed->imgs->getImages().size();
		cout << "feed " << _feed->id << " updated curr img index = " << _feed->curr_image << endl;
	}

	void nxtVideo(Feed* _feed) {
		if (_feed->vids)
			_feed->vids->nxtFeed();
	}

	void prevVideo(Feed* _feed) {
		if (_feed->vids)
			_feed->vids->prevFeed();
	}

	ofTexture* getFrameTexture(Feed* _feed) {

		//if its an active feed do the things
		if (checkFeed(*_feed)) {
			switch (_feed->media_type) {
			case IMAGE_COLLECTION:
			case IMAGE:
				(*_feed).isFrameNew = false;
				return &_feed->imgs->getImages()[_feed->curr_image];
				break;
			case VIDEO:
				return _feed->vids->getFrameTex();
				break;
			default:
				break;
			}
		}

		return NULL;
	}

	vector<ofTexture*> getFrameTextures(Feed* _feed, int amt) {

		vector<ofTexture*> texs;

		//if its an active feed do the things
		if (checkFeed(*_feed)) {

			switch (_feed->media_type) {
			case IMAGE_COLLECTION:
				for (int i = 0; i < amt; i++) {
					int index = (_feed->curr_image + i) % _feed->imgs->getImages().size();
					texs.push_back(&_feed->imgs->getImages()[index]);
				}
				break;
			case IMAGE:
				for (int i = 0; i < amt; i++)
					texs.push_back(&_feed->imgs->getImages()[_feed->curr_image]);
				break;
			case VIDEO:
				for (int i = 0; i < amt; i++)
					texs.push_back(_feed->vids->getFrameTex());
				break;
			}

		}

		(*_feed).isFrameNew = false;
		return texs;
	}

	vector<ofTexture*> getFrameTextures(Feed* _feed) {

		vector<ofTexture*> texs;
		int amt = (*_feed).collection_size;

		//if its an active feed do the things
		if (checkFeed(*_feed)) {
			switch (_feed->media_type) {
			case IMAGE_COLLECTION:
				for (int i = 0; i < amt; i++) {
					int index = (_feed->curr_image + i) % _feed->imgs->getImages().size();
					texs.push_back(&_feed->imgs->getImages()[index]);
				}
				break;
			case IMAGE:
				for (int i = 0; i < amt; i++)
					texs.push_back(&_feed->imgs->getImages()[_feed->curr_image]);
				break;
			case VIDEO:
				for (int i = 0; i < amt; i++)
					texs.push_back(_feed->vids->getFrameTex());
				break;
			}
		}

		(*_feed).isFrameNew = false;
		return texs;
	}

	void releaseFeed(Feed* _feed) {
		_feed->active = false;
		if (_feed->vids)_feed->vids->setActive(false);
		if (_feed->imgs)_feed->imgs->setActive(false);
		releaseHandlers();
	}

	void releaseHandlers() {
		for (int i = 0; i < images.size(); i++) {
			if (!images[i]->isActive())images.erase( images.begin() + i );
		}
		for (int i = 0; i < videos.size(); i++) {
			if (!videos[i]->isActive())videos.erase(videos.begin() + i);
		}
	}

	bool checkFeed(Feed& _feed) {
		return _feed.active;
	}	
	
	bool checkVideoHandler(VideoHandler& v) {
		return v.isActive();
	}	
	
	bool checkImageHandler(ImageHandler& i) {
		return i.isActive();
	}

	void initGui() {

		gui.setName("local media manager");
		//gui.add();
	}

	ofParameterGroup gui;

private:

	Feed feeds[20];		//fixed amount of possible feeds for better ptr access atm
	vector<VideoHandler*> videos;
	vector<ImageHandler*> images;

};
