#pragma once

#include "ofMain.h"

class ofxAfterEffectsKeyframeParser
{
public:
	
	class Track
	{
		friend class ofxAfterEffectsKeyframeParser;
		
	public:
		
		string getTrackName() const { return track_name; }
		
		size_t getNumParams() const { return param_name.size(); }
		string getParamName(size_t idx) const { return param_name.at(idx); }
		
		float getParam(float frame, const string& param_name, bool interpolated = true) const
		{
			if (param_name_map.find(param_name) == param_name_map.end())
			{
				ofLogError("ofxAfterEffectsKeyframeParser::Track::getParam") << "invalid param name";
				return 0;
			}
			if (data.empty()) return 0;

			int idx = param_name_map.at(param_name);

			auto it_upper = data.lower_bound(frame);

			if (!interpolated) {
				// returns nearest keyframe value
				if (it_upper == data.end()) { --it_upper; }
				return it_upper->second[idx];
			}

			// linear interporate
			if (it_upper == data.end()) {
				--it_upper;
				return it_upper->second[idx];
			}
			if (it_upper == data.begin()) {
				return it_upper->second[idx];
			}
			if (it_upper->first == frame) {
				return it_upper->second[idx];
			}

			auto it_lower = std::prev(it_upper);

			float frame0 = it_lower->first;
			float frame1 = it_upper->first;
			float value0 = it_lower->second[idx];
			float value1 = it_upper->second[idx];

			float t = (frame - frame0) / (frame1 - frame0);
			return value0 + (value1 - value0) * t;
		}
		
	protected:
		
		string track_name;
		vector<string> param_name;
		mutable map<string, int> param_name_map;
		mutable map<float, vector<float> > data;
	};
	
	void open(const string& path)
	{
		ofBuffer buf = ofBufferFromFile(path);
		parse(buf);
	}
	
	void dumpTrackName()
	{
		map<string, Track>::iterator it = tracks.begin();
		while (it != tracks.end())
		{
			cout << "trackname: '" << it->first << "'" << endl;
			
			cout << "params: { ";
			
			Track &t = it->second;
			for (int i = 0; i < t.getNumParams(); i++)
			{
				cout << "'" << t.getParamName(i) << "'";
				if (i != (t.getNumParams() - 1)) cout << ", ";
			}
			
			cout << " }" << endl << endl;
			
			it++;
		}
	}
	
	const Track& getTrack(const string& track_name) { return tracks[track_name]; }
	
	float getFirstFrame() const { return first_frame; }
	float getLastFrame() const { return last_frame; }

	float getSourceWidth()
	{
		 if( header.count("Source Width") > 0 ) { return header["Source Width"]; } else { return -1.0f; }
	}
	
	float getSourceHeight()
	{
		if( header.count("Source Height") > 0 ) { return header["Source Height"]; } else { return -1.0f; }
	}

	float getUnitsPerSecond()
	{
		if( header.count("Units Per Second") > 0 ) { return header["Units Per Second"]; } else { return -1.0f; }
	}

	float getCompPixelAspectRatio()
	{
		if( header.count("Comp Pixel Aspect Ratio") > 0 ) { return header["Comp Pixel Aspect Ratio"]; } else { return -1.0f; }
	}
	float getSourcePixelAspectRatio()

	{
		if( header.count("Source Pixel Aspect Ratio") > 0 ) { return header["Source Pixel Aspect Ratio"]; } else { return -1.0f; }
	}
	
protected:
	
	map<string, float> header;
	map<string, Track> tracks;
	
	float first_frame;
	float last_frame;
	
	void parse(ofBuffer &buf)
	{
		bool is_header = false;
		
		Track *current_track;
		
		first_frame = std::numeric_limits<float>::infinity();
		last_frame = -std::numeric_limits<float>::infinity();
		
		while (!buf.isLastLine())
		{
			string line = buf.getNextLine();
			
			if (line.empty()) continue;
			
			if (line[0] != '\t')
			{
				// statement line
				
				if (line == "Adobe After Effects 8.0 Keyframe Data" || line == "Adobe After Effects 9.0 Keyframe Data")
				{
					is_header = true;
					continue;
				}
				if (line == "End of Keyframe Data")
				{
					current_track = NULL;
					continue;
				}
				else
				{
					is_header = false;
					
					vector<string> e = ofSplitString(line, "\t", true, true);
					string track_name = ofJoinString(e, "/");
					
					tracks[track_name] = Track();
					current_track = &tracks[track_name];
					
					current_track->track_name = track_name;
				}
			}
			else
			{
				// data line
				
				if (is_header)
				{
					vector<string> e = ofSplitString(line, "\t", true, true);
					header[e[0]] = ofToFloat(e[1]);
				}
				else
				{
					vector<string> e = ofSplitString(line, "\t", false, true);
					if (e[1] == "Frame")
					{
						current_track->param_name = vector<string>(e.begin() + 2, e.end() - 1);
						
						for (int i = 0; i < current_track->param_name.size(); i++)
						{
							current_track->param_name_map[current_track->param_name[i]] = i;
						}
					}
					else
					{
						float frame = ofToFloat(e[1]);
						vector<float> &data = current_track->data[frame];
						
						first_frame = min(frame, first_frame);
						last_frame = max(frame, last_frame);
						
						for (int i = 2; i < e.size(); i++)
						{
							data.push_back(ofToFloat(e[i]));
						}
					}
				}
			}
		}
	}
};
