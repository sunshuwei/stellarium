/*
 * Stellarium
 * Copyright (C) 2006 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#ifndef TOASTMGR_HPP
#define TOASTMGR_HPP

#include "StelModule.hpp"

class ToastMgr : public StelModule
{
	Q_OBJECT
	Q_PROPERTY(bool surveyDisplayed
			READ getFlagShow
			WRITE setFlagShow
			NOTIFY surveyDisplayedChanged)
public:
	ToastMgr();
	~ToastMgr() override;
	void init() override;
	void deinit() override;
	void update(double deltaTime) override;
	void draw(StelCore* core) override;
	//! Used to determine the order in which the various modules are drawn. MilkyWay=1, we use 7 for actionDraw, else 0.
	double getCallOrder(StelModuleActionName actionName) const override;

public slots:
	void setFlagShow(bool displayed);
	bool getFlagShow(void) const;

signals:
	void surveyDisplayedChanged(const bool displayed) const;

private:
	class ToastSurvey* survey;
	class LinearFader* fader;
	bool displayNebulaTexturesFlag; // flag for following the state of deep-sky objects background images displaying
};

#endif // TOASTMGR_HPP
