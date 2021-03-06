#pragma once

#include <complex>
#include <iostream>
#include <string>
#include <vector>

#include "Model.h"

#define _VT 0.025851997

// Component Class
// Base class for component object, with basic constructor to set component name
// and the virtual prototypes for two functions, getNodes and getConductance
class Component {
public:
  Component(std::string p_name) : name{ p_name } {}

  std::string getName() {
    return name;
  }

  // getNodes Function
  // Implemented by each derived class, returns a vector of the integer IDs of
  // each node connected to the component
  virtual std::vector<int> getNodes();

  // getConductance Function
  // Implemented by each derived class, returns the conductance between two
  // nodes, so long as the component connects those nodes
  virtual std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency);

  // getProperties Function
  // Implemented by each derived class, returns a vector of component properties
  // to describe it's behaviour
  virtual std::vector<double> getProperties();

  // setProperties Function
  // Implemented by each derived class, allows internal values to be modified,
  // intended for use in changing companion model values in DC operating point
  // analysis
  virtual void setProperties(std::vector<double> properties);

protected:
  std::string name;
};

// ACCurrentSource Class
// Derived from Component, implements an AC current source
class ACCurrentSource : public Component {
public:
  ACCurrentSource(std::string p_name, double p_amplitude, double p_phase,
    int p_nodeIn, int p_nodeOut) : Component{ p_name }, nodeIn{ p_nodeIn },
    nodeOut{ p_nodeOut }, amplitude{ p_amplitude }, phase{ p_phase } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

private:
  double amplitude, phase;
  int nodeIn, nodeOut;
};

// DCCurrentSource Class
// Derived from Component, implements a DC current source
class DCCurrentSource : public Component {
public:
  DCCurrentSource(std::string p_name, double p_current, int p_nodeIn, int p_nodeOut) : 
    Component{ p_name }, current{ p_current }, 
    nodeIn{ p_nodeIn }, nodeOut{ p_nodeOut } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

private:
  double current;
  int nodeIn, nodeOut;
};

// VoltageControlledCurrentSource Class
// Derived from Component, implements a Voltage Controlled Current Source
class VoltageControlledCurrentSource : public Component {
public:
  VoltageControlledCurrentSource(std::string p_name, double p_transconductance,
    int p_nodeIn, int p_nodeOut, int p_control_nodeIn, int p_control_nodeOut) :
    Component{ p_name }, transconductance { p_transconductance }, nodeIn { p_nodeIn },
    nodeOut { p_nodeOut }, control_nodeIn { p_control_nodeIn }, control_nodeOut { p_control_nodeOut } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;
    
private:
  double transconductance;
  int nodeIn, nodeOut, control_nodeIn, control_nodeOut;
};

// ACVoltageSource Class
// Derived from Component, implements an AC voltage source
class ACVoltageSource : public Component {
public:
  ACVoltageSource(std::string p_name, double p_amplitude, double p_phase, int p_nodePlus,
    int p_nodeMinus) : Component{ p_name }, amplitude{ p_amplitude }, phase{ p_phase },
    nodePlus{ p_nodePlus }, nodeMinus{ p_nodeMinus } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

private:
  double amplitude, phase;
  int nodePlus, nodeMinus;
};

// DCVoltageSource Class
// Derived from Component, implements a DC voltage source
class DCVoltageSource : public Component {
public:
  DCVoltageSource(std::string p_name, double p_voltage, int p_nodePlus, int p_nodeMinus) :
    Component{ p_name }, voltage{ p_voltage }, nodePlus{ p_nodePlus },
    nodeMinus{ p_nodeMinus } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

private:
  double voltage;
  int nodePlus, nodeMinus;
};

// Resistor Class
// Derived from Component, implements a resistor
class Resistor : public Component{
public:
  Resistor(std::string p_name, double p_resistance, int p_node1, int p_node2) : 
    Component{ p_name }, resistance{ p_resistance }, node1{ p_node1 },
    node2{ p_node2 } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

private:
  double resistance;
  int node1, node2;
};

// Capacitor Class
// Derived from Component, implements a capacitor
class Capacitor : public Component{
public:
  Capacitor(std::string p_name, double p_capacitance, int p_node1, int p_node2) :
    Component{ p_name }, capacitance{ p_capacitance}, node1{ p_node1 },
    node2{ p_node2 } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

  private:
    double capacitance;
    int node1, node2;
};

// Inductor Class
// Derived from Component, implements a capacitor
class Inductor : public Component{
public:
  Inductor(std::string p_name, double p_inductance, int p_node1, int p_node2) :
    Component{ p_name }, inductance{ p_inductance}, node1{ p_node1 },
    node2{ p_node2 } {}

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

  private:
    double inductance;
    int node1, node2;
};

// Diode Class
// Derived from Component, implements a diode [in revision]
class Diode : public Component{
public:
  Diode(std::string p_name, int p_nodeAnode, int p_nodeCathode, Model* model);

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

  private:
    // Is - Reverse bias saturation current
    // Vd - Voltage across the diode (Vanode - Vcathode)
    // Gd - Conductance of companion model at current Vd
    // Id - Current of companion model at current Vd
    double Is, Vd, Gd, Id;
    int nodeAnode, nodeCathode;
    Model* model;
};

// BJT (both NPN and PNP) Class
// Derived from Component, implements a NPN or PNP BJT [in revision]
class BJT : public Component{
public:
  BJT(std::string p_name, int p_nodeCollector, int p_nodeBase, int p_nodeEmitter, Model* model);

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

  private:
    void updateConductancesAndCurrents();

    double Vbe, Vbc, Is, bf, br, Vaf, Var;
    double Gcc, Gcb, Gce, Gbc, Gbb, Gbe, Gec, Geb, Gee;
    double Ic, Ib, Ie;
    double Cjc, Vjc, Mjc, Cje, Vje, Mje, fc;
    double npn;
    int nodeCollector, nodeBase, nodeEmitter;
    Model* model;
};

// MOSFET Class
// Dervied from Compontent, implements a MOSFET
class MOSFET : public Component {
public: 
  MOSFET(std::string p_name, int p_nodeDrain, int p_nodeGate, int p_nodeSource, Model* model);

  std::vector<int> getNodes() override;
  std::complex<double> getConductance(int p_node1, int p_node2, double p_angularFrequency) override;
  std::vector<double> getProperties() override;
  void setProperties(std::vector<double> properties) override;

  private:
    int nodeDrain, nodeGate, nodeSource;
    double Vgs, Vds;
    double Id, Ig, Is;
    double vto, k, nmos, va;
    double Gdd, Gdg, Gds, Ggg, Ggd, Ggs, Gss, Gsd, Gsg;
    Model* model;
};
