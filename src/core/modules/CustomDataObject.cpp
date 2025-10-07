#include "CustomDataObject.hpp"
#include "StelPainter.hpp"

// Added by Kwantsin

CustomDataObject::CustomDataObject(const QString& codesignation, const Vec3d& coordJ2000, const bool isaMarker, bool showNames, float size, const QString& icon, const Vec3f& color)
	: CustomObject(codesignation, coordJ2000, isaMarker, icon)
	, customColor(color)
	, showName(showNames)
	, customSize(size)
{
}

void CustomDataObject::draw(StelCore* core, StelPainter* painter)
{
	CustomObject::_draw(core, painter, customColor, showName, customSize);
}
