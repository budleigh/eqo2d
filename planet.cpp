#include "planet.h"

void Planet::loadJson(json planetJson) {
	// access is a bit cumbersome here. the nl
	// json docs are a bit opaque, worth reading
	// more closely
	std::string jsonName = planetJson["name"];
	name = jsonName;

	std::string jsonType = planetJson["type"];
	type = jsonType == "gas" ? gas : rock;

	long cx = planetJson["coordinates"]["cx"];
	long cy = planetJson["coordinates"]["cy"];
	position.x = cx;
	position.y = cy;

	int jsonRadius = planetJson["radius"];
	radius = jsonRadius;

	// square box bounds for viewport checking
	// this sucks sam you're an idiot
	width = height = radius * 2;

	std::string jsonAssetFile = planetJson["assetFile"];
	assetFile = jsonAssetFile;
}
