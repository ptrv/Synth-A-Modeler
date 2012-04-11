// This DSP file has been generated by the Synth-A-Modeler compiler.
import("physicalmodeling.lib");

bigBlock(m1p,m2p,m3p,dev1p,adjStiffness) = (m1,m2,m3,dev1,a1,a2) with {
	l1 = (m1p - m2p) : link(adjStiffness,0.003,0.0);
	l2 = (m2p - m3p) : link(adjStiffness,0.004,0.0);
	l3 = (m2p - dev1p) : link(adjStiffness,0.005,0.0);

	m1 = (0.0-l1) : mass(0.01);
	m2 = (0.0+l1-l2-l3) : mass(1e-2);
	m3 = (0.0+l2) : mass(0.03);
	dev1 = (0.0+l3);

	a1 = 0.0+m1*(1000.0)+l1*(100.0);
	a2 = 0.0+l2*(1000.0)+l3*(-50.0)+l1*(0.01);
};

process = (bigBlock)~(_,_,_):(!,!,!,_,_,_);
