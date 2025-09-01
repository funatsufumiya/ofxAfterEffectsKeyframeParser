# ofxAfterEffectsKeyframeParser

Loads copy-pasted keyframe text data from After Effects.

Tested with oF v0.12.0

## Usage

- `test.txt` is copy-pasted keyframe text data.

```cpp
ofxAfterEffectsKeyframeParser aekey;
aekey.open("test.txt");

aekey.dumpTrackName();

const ofxAfterEffectsKeyframeParser::Track &t = aekey.getTrack("Motion Trackers/Tracker #1/Track Point #1/Attach Point");

for (int i = 0 ; i <= aekey.getLastFrame(); i++)
{
	cout << i << " " << t.getParam(i, "X pixels") << endl;
}
```

## Limitation

Easing (curve) data would be lost when copy-pasted.

If you need easing (curve), please consider [ofxAEEasingLoader](https://github.com/funatsufumiya/ofxAEEasingLoader/).
