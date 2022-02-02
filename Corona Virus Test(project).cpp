#include <iostream>
#include <stdlib.h>
#include <vector>
#include <set>
#include <time.h>
#include<dos.h> 
#include<conio.h>
#include <fstream>
#include <sstream>
#include <windows.h>
using namespace std;


void hospitalSearch(int source, vector<pair<string, vector<pair<int, int> > > > &v, int dist[], bool visited[], int prev[]) 
{
	for(int i = 0;i < v.size();i++)
	{
		visited[i] = false;
	}

	set<pair<int, pair<int, int> > > dq;
	
	dq.insert(make_pair(0, make_pair(-1, source)));
	
	while (!dq.empty())
	{
		//Pop curr from dq
		pair<int, pair<int, int> > c = *(dq.begin());
		dq.erase(dq.begin());
		
		if (visited[c.second.second]) continue;
		
		dist[c.second.second] = c.first;
		prev[c.second.second] = c.second.first;
		visited[c.second.second] = true;
		
		// Add adjacent nodes of curr to dq
		for(int i = 0;i < v[c.second.second].second.size();i++)
		{
			pair<int,int> cadj = v[c.second.second].second[i];
			if(!visited[cadj.first])
			{
				dq.insert(make_pair(c.first + cadj.second, make_pair(c.second.second, cadj.first)));
			}
			
		}		
	}
}

struct symptoms{
	string symName;
	int intensity;
};

int main()
{
	srand(time(NULL));
	
	ifstream m("Map.txt");
	
	int hospitals;
	
	m >> hospitals;
	
	int edges;
	
	m >> edges;
		
	vector<pair<string, vector<pair<int, int> > > > v(hospitals);
	int dist[hospitals], prev[hospitals];
	bool visited[hospitals] = { false };
	int labTestFee[hospitals];
	
	int from, to, weight;
	string hospitalName;
	
	for(int i = 1;i <= edges;i++) // storing map of hospitals
	{
		m >> hospitalName;
		
//		fflush(stdin);
		m >> from; 
		m >> to;
		m >> weight;
		
		v[from].first = hospitalName;
		v[from].second.push_back(make_pair(to, weight));
		v[to].second.push_back(make_pair(from, weight));
		
		m >> hospitalName;
//		fflush(stdin);
		
		v[to].first = hospitalName;
	}
	
	for(int i = 0;i < hospitals;i++)
	{
		if(i == 0 || i == 4 || i == 12 || i == 19)
		{
			labTestFee[i] = -1;
			continue;
		}
		
		m >> labTestFee[i];
	}
	
	m.close();
	
	vector<pair<int, int> > numOfBeds(hospitals);
	
	for(int i = 0;i < hospitals;i++)
	{
		if(i == 0 || i == 4 || i == 12 || i == 19)
		{
			numOfBeds[i].first = 0;
			numOfBeds[i].second = 0;
			continue;
		}
		
		numOfBeds[i].second = rand() % 21;
		
		if(numOfBeds[i].second == 0)
		{
			numOfBeds[i].second += 10;
			numOfBeds[i].first = rand() % (numOfBeds[i].second + 1);
		}
	}
	
	vector<string> dis;
	dis.push_back("Drycough");
	dis.push_back("Sorethroat");
	dis.push_back("Highfever");
	dis.push_back("Headache");
	
	ifstream FileReading("Patient.csv");
	if(FileReading.is_open())
	{
//		cout << "Opened successfully" << endl;
		string FirstLine;
		FileReading >> FirstLine;
//		cout << FirstLine << endl;
//		while(!FileReading.eof())
		for(int cou=0;cou<500;cou++)
		{
			int price;
			string area;
			string travelling;
			int patientID;
			FileReading >> patientID;
//			cout << patientID;
			char dispose;
			FileReading >> dispose;
			char read;
			vector<symptoms> arr;
			int k=0;
			while(1)
			{
				string symInten;
				FileReading >> symInten;
//				cout << symInten << endl;
				if(symInten=="end,")
					break;	
				// breaking the current string
				string symName;
				int Intensity=0;
				int i=0;
				for(i=0;i<symInten.size();i++)
				{
					if(symInten[i]!='_')
						symName+=symInten[i];
					else
						break;
				}
//				cout << symName;
				string sev;
				i++;
				for(i;i<symInten.size();i++)
				{
					sev+=symInten[i];
				}
//				cout << " " << sev << endl;
				stringstream convert(sev);
				convert >> Intensity;
//				cout << " " << Intensity << endl;
				arr.push_back(symptoms());
				arr[k].symName=symName;
				arr[k].intensity=Intensity;
				
				k++;
			}
			FileReading >> travelling;
			travelling[travelling.size()-1]=' ';
//			cout << recom;
			FileReading >> price;
//			cout << price;
			FileReading >> dispose;
			FileReading >> area;
//			cout << area;
			
//			cout << disposed << endl;
			cout << "================================================" << endl;
			cout << "Patient ID: " << patientID << endl;
			cout << "SYMPTOMS:  " << endl;
			for(int j=0;j<arr.size();j++)
			{
				
				cout << j+1 << " ->   " << arr[j].symName << " " << arr[j].intensity << endl;
//				cout << "================================================" << endl;
			}
			cout << endl;
			cout << "Traveling History: " << travelling << endl; 
			cout << "Test Affordibility: " << price << endl;
			cout << "Area: " << area << endl;
			bool isPresent;
			for(int i=0;i<dis.size();i++)
			{
				isPresent=false;
				string disName=dis[i];
				for(int j=0;j<arr.size();j++)
				{
					if(disName==arr[j].symName && arr[j].intensity>=50)
					{
						isPresent=true;
						break;
					}
				}
				if(isPresent)
					continue;
				else
					break;
			}
			cout << endl;
			if(isPresent)
			{
				cout << "Corona Test Recommended" << endl << endl ;
				
				cout << "Hospitals within test affordability of patient:" << endl << endl;
				
				if(area == "PECHS")
				{
					hospitalSearch(0, v, dist, visited, prev);
				}
				
				else if(area == "Gulshan")
				{
					hospitalSearch(4, v, dist, visited, prev);
				}
				
				else if(area == "Nazimabad")
				{
					hospitalSearch(12, v, dist, visited, prev);
				}
				
				else
				{
					hospitalSearch(20, v, dist, visited, prev);
				}
				
				for(int i = 0;i < hospitals;i++)
				{
					if(labTestFee[i] > -1 && labTestFee[i] <= price && numOfBeds[i].first < numOfBeds[i].second)
					{
						cout << v[i].first << " : " << dist[i] << endl;
					}
				}
				
				cout << endl;
			}	
			else
				cout << "Corona Test Not Recommended" << endl ;
				
			cout << "================================================" << endl;
			Sleep(2000);

		}
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
}
