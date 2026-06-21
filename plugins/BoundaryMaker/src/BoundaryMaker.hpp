/*
 * Constellation Boundary Maker plug-in for Stellarium
 *
 * Copyright (C) 2025 Your Name
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOUNDARYMAKER_HPP
#define BOUNDARYMAKER_HPP

#include "StelModule.hpp"
#include "StelGui.hpp"
#include <QObject>
#include <QString>
#include <QList>
#include <QPair>

class BoundaryMakerDialog;
class StelButton;

class BoundaryMaker : public StelModule
{
	Q_OBJECT
	Q_PROPERTY(bool enabled READ getEnabled WRITE setEnabled NOTIFY enabledChanged)
public:
	BoundaryMaker();
	~BoundaryMaker() override;

	///////////////////////////////////////////////////////////////////////////
	// Methods defined in the StelModule class
	void init() override;
	void deinit() override;
	void update(double deltaTime) override;
	void draw(StelCore* core) override;
	double getCallOrder(StelModuleActionName actionName) const override;

	// Get available constellation abbreviations from current sky culture
	QStringList getConstellationAbbreviations() const;

	// Get default epoch for boundaries
	QString getDefaultEpoch() const { return "J2000.0"; }

	// Get/Set enabled state
	bool getEnabled() const { return enabled; }
	void setEnabled(bool value);

signals:
	void enabledChanged(bool enabled);

private slots:
	void showDialog();

private:
	BoundaryMakerDialog* dialog;
	StelButton* toolbarButton;
	bool enabled;
	static const QString actionId;
	static const QString groupId;
};

#include "StelPluginInterface.hpp"

class BoundaryMakerStelPluginInterface : public QObject, public StelPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID StelPluginInterface_iid)
	Q_INTERFACES(StelPluginInterface)
public:
	StelModule* getStelModule() const override;
	StelPluginInfo getPluginInfo() const override;
	QObjectList getExtensionList() const override { return QObjectList(); }
};

#endif /* BOUNDARYMAKER_HPP */
