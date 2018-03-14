/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Open Source Modelica Consortium (OSMC),
 * c/o Linköpings universitet, Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS OSMC PUBLIC LICENSE (OSMC-PL) VERSION 1.2.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL VERSION 3,
 * ACCORDING TO RECIPIENTS CHOICE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from OSMC, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or
 * http://www.openmodelica.org, and in the OpenModelica distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */

#include "Connector.h"
#include "ssd/ConnectorGeometry.h"
#include "Logging.h"

#include <cstring>

oms2::Connector::Connector(oms_causality_enu_t causality, oms_signal_type_enu_t type, const oms2::SignalRef& name)
{
  this->causality = causality;
  this->type = type;

  std::string str = name.toString();
  this->name = new char[str.size()+1];
  strcpy(this->name, str.c_str());

  this->geometry = NULL;
}

oms2::Connector::Connector(oms_causality_enu_t causality, oms_signal_type_enu_t type, const oms2::SignalRef& name, double height)
{
  this->causality = causality;
  this->type = type;

  std::string str = name.toString();
  this->name = new char[str.size()+1];
  strcpy(this->name, str.c_str());

  double x, y;
  switch (causality)
  {
    case oms_causality_input:
      // inputs are placed on the left of the component
      x = 0.0;
      break;
    case oms_causality_output:
      // outputs are placed on the right of the component
      x = 1.0;
      break;
    default:
      // parameters are placed on the top of the component
      x = height;
      y = 1.0;
      break;
  }
  this->geometry = reinterpret_cast<ssd_connector_geometry_t*>(new oms2::ssd::ConnectorGeometry(x, y));
}

oms2::Connector::~Connector()
{
  if (this->name) delete[] this->name;
  if (this->geometry) delete reinterpret_cast<oms2::ssd::ConnectorGeometry*>(this->geometry);
}

oms2::Connector::Connector(const oms2::Connector& rhs)
{
  this->causality = rhs.causality;
  this->type = rhs.type;

  this->name = new char[strlen(rhs.name)+1];
  strcpy(this->name, rhs.name);

  if (rhs.geometry)
    *reinterpret_cast<oms2::ssd::ConnectorGeometry*>(this->geometry) = *reinterpret_cast<oms2::ssd::ConnectorGeometry*>(rhs.geometry);
  else
    this->geometry = NULL;
}

oms2::Connector& oms2::Connector::operator=(const oms2::Connector& rhs)
{
  // check for self-assignment
  if(&rhs == this)
    return *this;

  this->causality = rhs.causality;
  this->type = rhs.type;

  if (this->name)
    delete[] this->name;
  this->name = new char[strlen(rhs.name)+1];
  strcpy(this->name, rhs.name);

  this->setGeometry(reinterpret_cast<oms2::ssd::ConnectorGeometry*>(rhs.geometry));

  return *this;
}

void oms2::Connector::setName(const oms2::SignalRef& name)
{
  if (this->name)
    delete[] this->name;

  std::string str = name.toString();
  this->name = new char[str.size()+1];
  strcpy(this->name, str.c_str());
}

void oms2::Connector::setGeometry(const oms2::ssd::ConnectorGeometry* newGeometry)
{
  if (this->geometry)
  {
    delete reinterpret_cast<oms2::ssd::ConnectorGeometry*>(this->geometry);
    this->geometry = NULL;
  }

  if (newGeometry)
    *reinterpret_cast<oms2::ssd::ConnectorGeometry*>(this->geometry) = *newGeometry;
}