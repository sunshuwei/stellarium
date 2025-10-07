#ifndef CUSTOMDATAOBJECT_HPP
#define CUSTOMDATAOBJECT_HPP

#include <CustomObject.hpp>

// Added by Kwantsin

class CustomDataObject : public CustomObject
{
public:
	CustomDataObject(const QString& codesignation, const Vec3d& coordJ2000, const bool isaMarker, bool showName, float size, const QString& icon = "cross", const Vec3f& color = Vec3f(0.1f, 1.0f, 0.1f));
	virtual void draw(StelCore* core, StelPainter* painter) override;

private:
	Vec3f customColor;
	bool showName;
	float customSize;
};


#endif // CUSTOMDATAOBJECT_HPP