#pragma once
class AbstractFigure
{
	virtual void CreateModel(float radius, int textures_count) = 0;
	virtual void setBuffer() = 0;
};

