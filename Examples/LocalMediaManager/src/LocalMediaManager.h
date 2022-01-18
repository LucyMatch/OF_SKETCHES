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
	int id;											/// \ID & index
	mediaTypes media_type;							/// \Media type
	string path;									/// \Path to media directory
	bool enable_slideshow = false;					/// \If true image file updated to next in directory based on freq / is false static image
	int slideshow_frequency = 5;					/// \Frames between Image updates
	bool isFrameNew = false;						/// \True if media has been updated
	VideoHandler* vids;								/// \Pointer to videohandler obj
	ImageHandler* imgs;								/// \Pointer to imageHandler obj
	int update_counter = 0;							/// \Running counter of updates - for slideshow frequency
	int curr_image = 0;								/// \Current Image Index
	int collection_size = 1;						/// \How many images are in a collection
	bool dead = false;
};

class LocalMediaManager {

public:

	LocalMediaManager() {};

	Feed* createNewFeed(const Feed& new_feed) {

		Feed _feed = new_feed;

		//sort out the media handler
		if (_feed.media_type < 2) {
			////image based types
			images.push_back(new ImageHandler(_feed.path));
			_feed.imgs = images.back();
		}
		else if (_feed.media_type == 2) {
			//video based types
			VideoHandler* vh = new VideoHandler(glm::vec2(1920, 1080));
			vh->setup(_feed.path, VideoHandler::VIDEO_LOCAL);
			vh->setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
			videos.push_back(vh);
			_feed.vids = videos.back();
		}

		//set id
		_feed.id = feeds.size();

		//add to array
		feeds.push_back(_feed);

		//return pointer of feed
		return &feeds[_feed.id];
	}
	
	void update() {

		for (auto v : videos)
			v->update();

		for (auto &f : feeds) {

			f.update_counter++;

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

		checkFeeds();

	}

	void drawDebug() {

		int amt = feeds.size();
		int cols = 3;
		int rows = glm::ceil(amt / cols);
		glm::vec2 pos(0, 0);
		int counter = 0;

		for (auto f : feeds) {

			if (counter % cols != 0) pos.x = ofGetWidth() /cols ;
			pos.y = glm::floor((counter / cols)) * (ofGetHeight() / 2);

			switch (f.media_type) {
			case VIDEO :
				f.vids->getFrameTex()->draw(pos.x,pos.y);
				break;
			case IMAGE :
				f.imgs->getImages()[f.curr_image].draw(pos.x, pos.y);
				break;
			case IMAGE_COLLECTION :

					int cols = 2;
					int rows = glm::ceil(f.collection_size / cols);
					glm::vec2 nested_pos(0,0);
				for (int i = 0; i < f.collection_size; i++) {
					ofPushMatrix();
					ofPushStyle();

					ofTranslate( nested_pos.x, nested_pos.y );
					ofScale(0.25);

					f.imgs->getImages()[f.curr_image].draw(pos.x, pos.y);
					nxtImage(&f);

					ofPopStyle();
					ofPopMatrix();
				}

				break;
			}
			counter++;
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

		switch (_feed->media_type) {
		case IMAGE_COLLECTION:
		case IMAGE:
			//(*_feed).isFrameNew = false;
			return &_feed->imgs->getImages()[_feed->curr_image];
			break;
		case VIDEO:
				return _feed->vids->getFrameTex();
			break;
		default:
				break;
		}

		return new ofTexture;
	}

	vector<ofTexture*> getFrameTextures(Feed* _feed, int amt) {

		vector<ofTexture*> texs;

		switch (_feed->media_type) {
		case IMAGE_COLLECTION:
			for (int i = 0; i < amt; i++) {
				texs.push_back(&_feed->imgs->getImages()[_feed->curr_image]);
				nxtImage(_feed);
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

		(*_feed).isFrameNew = false;
		return texs;
	}

	void deleteFeed(Feed* _feed) {
		_feed->dead = true;
		if (_feed->vids)_feed->vids->deactivate = true;
		if (_feed->imgs)_feed->imgs->deactivate = true;
	}

	void checkFeeds() {
		for (int i = 0; i < videos.size(); i++) {
			if (checkVideoHandler(*videos[i]))
				videos.erase(videos.begin() + i);
		}
		for (int i = 0; i < images.size(); i++) {
			if (checkImageHandler(*images[i]))
				images.erase(images.begin() + i);
		}
		for (int i = 0; i < feeds.size(); i++) {
			if (checkFeed(feeds[i]))
				feeds.erase(feeds.begin() + i);
		}
	}
	
	bool checkFeed(Feed& _feed) {
		return _feed.dead;
	}	
	
	bool checkVideoHandler(VideoHandler& v) {
		return v.deactivate;
	}	
	
	bool checkImageHandler(ImageHandler& i) {
		return i.deactivate;
	}

	void initGui() {

		gui.setName("local media manager");
		//gui.add();
	}

	ofParameterGroup gui;

private:

	vector<Feed> feeds;
	vector<VideoHandler*> videos;
	vector<ImageHandler*> images;

};
