#include <iostream>
#include<fstream>
#include <iomanip>
#include <vector>
#include <ctime>
using namespace std;

int main()
{
	double const RHO = 1.225;	// air density
	double const PI = 3.14159265;
	double const g = 9.81;		// Gravitational acceleretaion

	double KA = 0.4;			// Resistant coefficient for element of mass of Rope
	double KAend = 0.5;			// Resistant coefficient for mass at the end of Rope
	double R = 0.02;			// Radious of Rope section
	double Srope = PI * R * R ;	// Square of Rope section
	double Rend = 0.05;			// Radious of end mass section
	double Send = PI * Rend * Rend;	//Square of end mass section
	
	//Uncomment if you want enter data from console 
	/*int NUM;
	cout << "Enter Number of points:" << endl;
	cin >> NUM;
	double length;
	cout << "Enter length of interval:" << endl;
	cin >> length;
	double mass;
	cout << "Enter mass of interval:" << endl;
	cin >> mass;
	double endMass;
	cout << "Enter mass at the END of Rope (0 if nothing):" << endl;
	cin >> endMass;
	double airvel;
	cout << "Enter velocity of airflow:" << endl;
	cin >> airvel;
	double gamma;
	cout << "Enter angle of airflow :" << endl;
	cin >> gamma;*/

	// Test values
	int NUM = 20;			// Number of point masses of Rope
	double length = 0.05;	// Length of point mass of Rope in m
	double mass = 0.02;		// Mass of point element of Rope in kg
	double endMass = 0.2;	// Mass at the end element of Rope in kg
	double airvel = 30;		// Speed of Air Flow in m/s
	double gamma = 10;		// Angle of Air Flow with respect to horizen
	

	vector<double> vcos(NUM);
	vector<double> vsin(NUM);
	vector<double> xcoor(NUM);
	vector<double> ycoor(NUM);

	double gammaRad = gamma * PI / 180;	// Angle of airflow with respect to horizon (minus if down, plus if up) 
	double flowCos = cos(gammaRad);
	double flowSin = sin(gammaRad);
	double AirForce = KA * RHO * airvel * airvel * Srope / 2;	// Airflow resistance of element of Rope
	double Weight = g * mass;						// Weight of element of Rope
	double RXconst = AirForce * flowCos;			// Projection of Force to X axis
	double RYconst = -Weight + AirForce * flowSin;	// Projection of Force to Y axis
	double RX;
	double RY;
	double RTOT;
	double AirForceEnd; // Airflow resistance of element at the end of Rope
	double endWeight;  // Weight of element at the end of Rope
	if (endMass == 0)
	{
		AirForceEnd = AirForce;
		endWeight = Weight;
	}
	else
	{
		AirForceEnd = KAend * RHO * airvel * airvel * Send / 2;
		endWeight = g * endMass;
	}

	for (int i = NUM-1; i >= 0; i--)
	{
		if (i == NUM-1) 
		{
			RX = AirForceEnd * flowCos;
			RY = -endWeight + AirForceEnd * flowSin;
			RTOT = sqrt(RX*RX + RY*RY);
			vcos[i] = RY / RTOT;
			vsin[i] = RX / RTOT;
		}
		else
		{
			RX = RXconst + RX;
			RY = RYconst + RY;
			RTOT = sqrt(RX * RX + RY * RY);
			vcos[i] = RY / RTOT;
			vsin[i] = RX / RTOT;
		}
	}
	
	srand(time(NULL));
	for (int i = 0; i < NUM; i++)
	{
		if (i == 0)
		{
			xcoor[i] = length * vsin[i];
			ycoor[i] = length * vcos[i];
		}
		else
		{
			xcoor[i] = xcoor[i-1] + length * vsin[i];
			ycoor[i] = ycoor[i-1] + length * vcos[i];
			
		}
	}

	cout << endl;
	cout << "Length of Rope: " << length * NUM << " m" << endl;
	cout << "Mass of Rope: " << mass * (NUM - 1) + endWeight/g << " kg" << endl;
	cout << "Air Flow speed: " << airvel << " m/s, Angle of Air Flow: " << gamma << " deg" <<  endl;
	cout << "Coordinates of points of Rope" << endl;
	cout << " Angle" << " \t \t " << "X" << " \t \t " << "Y" << endl;
	for (int i = 0; i < NUM; i++)
	{
		cout << fixed << setprecision(6) << asin(vsin[i])*180/PI << " \t " << xcoor[i] << " \t " << ycoor[i] << endl;
	}

	string path = "output.dat";
	ofstream fout;
	fout.open(path);

	if(!fout.is_open())
	{
		cout << "Open file ERROR" << endl;
	}
	else {
		cout << endl << "The coordinates are written in the file output.dat" << endl;
		fout << fixed << setprecision(6) << 0.0 << " \t " << 0.0 << endl;
		for (int i = 0; i < NUM; i++)
		{
			//cout << xcoor[i] << " \t " << ycoor[i] << endl;
			fout << fixed << setprecision(6) << xcoor[i] << " \t " << ycoor[i] << endl;
		}
	}
	fout.close();

	return 0;
}