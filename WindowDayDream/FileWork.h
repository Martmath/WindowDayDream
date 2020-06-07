#pragma once
template<class T, bool InitZero = true>
bool loadFrom(int n, T& dX, T& dY, T& dZ, T& kX, T& kY, T& kZ)
{
	const char delimiter = ',';
	ifstream myFile("data.txt", ifstream::in);

	if constexpr (InitZero)
	{
		dX = 0.0; dY = 0.0; dZ = 0.0; kX = 1.0; kY = 1.0; kZ = 1.0;
	}

	if (myFile.fail()) return false;
	string stmp;
	for (int i = 0; i < n; ++i)
	{
		if (!getline(myFile, stmp))	return false;
	}

	if (!getline(myFile, stmp, delimiter)) return false;
	dX = atof(stmp.c_str());
	getline(myFile, stmp, delimiter);
	dY = atof(stmp.c_str());
	getline(myFile, stmp, delimiter);
	dZ = atof(stmp.c_str());
	getline(myFile, stmp, delimiter);
	kX = atof(stmp.c_str());
	getline(myFile, stmp, delimiter);
	kY = atof(stmp.c_str());
	getline(myFile, stmp);
	kZ = atof(stmp.c_str());
	return true;
}


template<class T>
void saveTo(int n, T& dX, T& dY, T& dZ, T& kX, T& kY, T& kZ)
{
	string ee = to_string(dX) + "," + to_string(dY) + "," + to_string(dZ) + "," +
		to_string(kX) + "," + to_string(kY) + "," + to_string(kZ);
	string bb = "0,0,0,1,1,1";

	ifstream myFile("data.txt", ifstream::in);
	std::vector<std::string> lines;
	string line;
	//int n = 0;
	if (myFile.good())
	{
		while (getline(myFile, line))
		{
			lines.push_back(line);
			//n++;
		}
	}
	myFile.close();
	//int n_1 = n + 1;
	int N = lines.size();
	if (N < (n + 1))
	{
		for (int i = 0; i < (n - N); ++i) lines.push_back(bb);
		lines.push_back(ee);
	}
	else
	{
		lines[n] = ee;
	}
	int rr = 9;

	ofstream ofs("data.txt", std::ofstream::out | std::ofstream::trunc);

	for (auto& s : lines)
		ofs << s << std::endl;
	ofs.close();
}
