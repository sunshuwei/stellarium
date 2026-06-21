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

#include "BoundaryMaker.hpp"
#include "BoundaryMakerDialog.hpp"
#include "StelApp.hpp"
#include "StelCore.hpp"
#include "StelModuleMgr.hpp"
#include "StelSkyCultureMgr.hpp"
#include "StelObjectMgr.hpp"
#include "Constellation.hpp"
#include "ConstellationMgr.hpp"
#include "StelGui.hpp"
#include "StelGuiItems.hpp"
#include <QDebug>
#include <QPixmap>

const QString BoundaryMaker::actionId = "actionShow_Boundary_Maker";
const QString BoundaryMaker::groupId = "Boundary Maker";

StelModule* BoundaryMakerStelPluginInterface::getStelModule() const
{
	return new BoundaryMaker();
}

StelPluginInfo BoundaryMakerStelPluginInterface::getPluginInfo() const
{
	StelPluginInfo info;
	info.id = "BoundaryMaker";
	info.displayedName = "Boundary Maker";
	info.authors = "Your Name";
	info.contact = "your@email.com";
	info.description = "A tool for creating constellation boundaries.";
	return info;
}

BoundaryMaker::BoundaryMaker() : dialog(nullptr), toolbarButton(nullptr), enabled(false)
{
	setObjectName("BoundaryMaker");
}

BoundaryMaker::~BoundaryMaker()
{
	if (dialog)
	{
		delete dialog;
		dialog = nullptr;
	}
	if (toolbarButton)
	{
		delete toolbarButton;
		toolbarButton = nullptr;
	}
}

void BoundaryMaker::init()
{
	qDebug() << "Initializing BoundaryMaker plugin";
	
	// Create the main dialog
	dialog = new BoundaryMakerDialog(this);
	
	// Add action to show the dialog (using lambda to avoid SLOT macro issues in static plugins)
	addAction(actionId, groupId, N_("Boundary Maker"), this, [this]() { showDialog(); }, "Ctrl+B", "showBoundaryMaker");
	
	// Add a toolbar button
	try
	{
		StelApp &app = StelApp::getInstance();
		StelGui *gui = dynamic_cast<StelGui *>(app.getGui());
		if (gui != Q_NULLPTR)
		{
			// Use constellation boundary icons
			QPixmap iconEnabled(":/graphicGui/btConstellationBoundaries-on.png");
			QPixmap iconDisabled(":/graphicGui/btConstellationBoundaries-off.png");
			QPixmap glow(":/graphicGui/miscGlow32x32.png");
			
			qDebug() << "BoundaryMaker: iconEnabled valid:" << !iconEnabled.isNull();
			qDebug() << "BoundaryMaker: iconDisabled valid:" << !iconDisabled.isNull();
			qDebug() << "BoundaryMaker: glow valid:" << !glow.isNull();
			
			if (!iconEnabled.isNull() && !iconDisabled.isNull())
			{
				// StelButton constructor: parent, pixOn, pixOff, pixNoChange, pixHover, actionId, noBackground, isTristate
				toolbarButton = new StelButton(Q_NULLPTR, iconEnabled, iconDisabled, QPixmap(), glow, actionId, false, false);
				gui->getButtonBar()->addButton(toolbarButton, "065-pluginsGroup");
				qDebug() << "BoundaryMaker: Toolbar button added successfully";
			}
			else
			{
				qWarning() << "BoundaryMaker: Failed to load icons";
			}
		}
		else
		{
			qWarning() << "BoundaryMaker: GUI is null";
		}
	}
	catch (const std::exception& e)
	{
		qWarning() << "BoundaryMaker: Failed to create toolbar button:" << e.what();
	}
	catch (...)
	{
		qWarning() << "BoundaryMaker: Failed to create toolbar button (unknown exception)";
	}
}

void BoundaryMaker::deinit()
{
	if (dialog)
	{
		delete dialog;
		dialog = nullptr;
	}
	if (toolbarButton)
	{
		delete toolbarButton;
		toolbarButton = nullptr;
	}
}

void BoundaryMaker::update(double deltaTime)
{
	Q_UNUSED(deltaTime);
}

void BoundaryMaker::draw(StelCore* core)
{
	Q_UNUSED(core);
}

double BoundaryMaker::getCallOrder(StelModuleActionName actionName) const
{
	if (actionName == StelModule::ActionDraw)
		return StelApp::getInstance().getModuleMgr().getModule("NebulaMgr")->getCallOrder(actionName) + 10.;
	return 0;
}

QStringList BoundaryMaker::getConstellationAbbreviations() const
{
	QStringList abbreviations;
	
	try {
		// Get object manager to find constellations
		StelObjectMgr* objMgr = GETSTELMODULE(StelObjectMgr);
		if (!objMgr)
		{
			qWarning() << "BoundaryMaker: StelObjectMgr is null";
			return abbreviations;
		}
		
		// Get all constellations using listAllModuleObjects
		QVector<QPair<QString, StelObjectP>> objects = objMgr->listAllModuleObjects("ConstellationMgr", true);
		qDebug() << "BoundaryMaker: Found" << objects.size() << "constellation objects";
		
		for (const auto& pair : objects)
		{
			Constellation* cons = dynamic_cast<Constellation*>(pair.second.data());
			if (cons)
			{
				QString id = cons->getID();
				qDebug() << "BoundaryMaker: Found constellation ID:" << id;
				// Extract abbreviation from ID (e.g., "CON modern AND" -> "AND" or "CON chinese_mdn P24" -> "P24")
				QStringList parts = id.split(" ");
				if (parts.size() >= 3)
				{
					QString abbr = parts.last();
					if (!abbreviations.contains(abbr))
					{
						abbreviations.append(abbr);
					}
				}
			}
		}
		
		qDebug() << "BoundaryMaker: Found" << abbreviations.size() << "constellation abbreviations";
	} catch (const std::exception& e) {
		qWarning() << "BoundaryMaker: Exception in getConstellationAbbreviations:" << e.what();
	} catch (...) {
		qWarning() << "BoundaryMaker: Unknown exception in getConstellationAbbreviations";
	}
	
	if (abbreviations.isEmpty()) {
		// Fallback: use some common constellation abbreviations
		abbreviations << "AND" << "PEG" << "CAS" << "CEP" << "LYR" << "AQU" << "PIC";
		qDebug() << "BoundaryMaker: Using fallback constellation list";
	}
	
	abbreviations.sort();
	return abbreviations;
}

void BoundaryMaker::setEnabled(bool value)
{
	enabled = value;
	emit enabledChanged(enabled);
}

void BoundaryMaker::showDialog()
{
	qDebug() << "BoundaryMaker::showDialog() called";
	if (dialog)
	{
		qDebug() << "BoundaryMaker: dialog exists, showing...";
		dialog->setVisible(true);
		qDebug() << "BoundaryMaker: dialog shown";
	}
	else
	{
		qDebug() << "BoundaryMaker: dialog is null!";
	}
}
