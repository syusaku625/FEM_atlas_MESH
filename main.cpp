#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>

using namespace std;

void export_vtu(const std::string &file, vector<vector<int>> element, vector<vector<double>> node, vector<double> C)
{
  int numOfNode=node.size();
  int numOfElm=element.size();
  
  FILE *fp;
  if ((fp = fopen(file.c_str(), "w")) == NULL)
  {
    cout << file << " open error" << endl;
    exit(1);
  }

  fprintf(fp, "<VTKFile type=\"UnstructuredGrid\" version=\"1.0\" byte_order=\"LittleEndian\" header_type=\"UInt32\">\n");
  fprintf(fp, "<UnstructuredGrid>\n");
  fprintf(fp, "<Piece NumberOfPoints= \"%d\" NumberOfCells= \"%d\" >\n", numOfNode, numOfElm);
  fprintf(fp, "<Points>\n");
  int offset = 0;
  fprintf(fp, "<DataArray type=\"Float64\" Name=\"Position\" NumberOfComponents=\"3\" format=\"appended\" offset=\"%d\"/>\n",offset);
  offset += sizeof(int) + sizeof(double) * numOfNode * 3;
  fprintf(fp, "</Points>\n");

  fprintf(fp, "<Cells>\n");
  fprintf(fp, "<DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">\n");
  for (int i = 0; i < numOfElm; i++){
    for (int j = 0; j < static_cast<int>(element[i].size()); j++) fprintf(fp, "%d ", element[i][j]);
    fprintf(fp, "\n");
  }
  fprintf(fp, "</DataArray>\n");
  fprintf(fp, "<DataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\">\n");
  int num = 0;
  for (int i = 0; i < numOfElm; i++)
  {
    num += element[i].size();
    fprintf(fp, "%d\n", num);
  }
  fprintf(fp, "</DataArray>\n");
  fprintf(fp, "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n");
  for (int i = 0; i < numOfElm; i++) fprintf(fp, "%d\n", 10);
    
  fprintf(fp, "</DataArray>\n");
  fprintf(fp, "</Cells>\n");

  fprintf(fp, "<PointData>\n");
  fprintf(fp, "</PointData>\n");

  fprintf(fp, "<CellData>\n");
  fprintf(fp, "<DataArray type=\"Float64\" Name=\"pressure[Pa]\" NumberOfComponents=\"1\" format=\"appended\" offset=\"%d\"/>\n",offset);
  offset += sizeof(int) + sizeof(double) * numOfElm;
  fprintf(fp, "</CellData>\n");
  fprintf(fp, "</Piece>\n");
  fprintf(fp, "</UnstructuredGrid>\n");
  fprintf(fp, "<AppendedData encoding=\"raw\">\n");
  fprintf(fp, "_");
  fclose(fp);

  fstream ofs;
  ofs.open(file.c_str(), ios::out | ios::app | ios_base::binary);
  double *data_d = new double[numOfNode*3];
  num = 0;
  int size=0;
  for (int ic = 0; ic < numOfNode; ic++){
    data_d[num] = node[ic][0];
    num++;
    data_d[num] = node[ic][1];
    num++;
    data_d[num] = node[ic][2];
    num++;
  }
  size=sizeof(double)*numOfNode*3;
  ofs.write((char *)&size, sizeof(size));
  ofs.write((char *)data_d, size);

  double *data_d_2 = new double[numOfElm];
  num=0;
  for (int ic = 0; ic < numOfElm; ic++){
      data_d_2[num]   = C[ic];
      num++;
  }

  size=sizeof(double)*numOfElm;
  ofs.write((char *)&size, sizeof(size));
  ofs.write((char *)data_d_2, size);

  delete[] data_d;

  ofs.close();

  if ((fp = fopen(file.c_str(), "a")) == NULL)
  {
    cout << file << " open error" << endl;
    exit(1);
  }
  fprintf(fp, "\n</AppendedData>\n");
  fprintf(fp, "</VTKFile>\n");
  fclose(fp);
}

int main()
{
    ifstream ifs("node.dat");
    string str;
    
    vector<vector<double>> x;

    while(getline(ifs,str)){
        istringstream ss2(str);
        vector<double> tmp_x;
        for(int j=0; j<3; j++){
            getline(ss2,str,' ');
            tmp_x.push_back(stod(str));
        }
        x.push_back(tmp_x);
    }
    ifs.close();
    cout << x.size() << endl;

    ifs.open("element.dat");
    vector<vector<int>> element;
    vector<double> judge;
    while(getline(ifs,str)){
        istringstream ss2(str);
        vector<int> tmp_element;
        for(int j=0; j<5; j++){
            getline(ss2, str, ' ');
            if(j==4){
                judge.push_back(stod(str));
                cout << str << endl;
                break;
            }
            tmp_element.push_back(stoi(str)-1);
        }
        element.push_back(tmp_element);
    }

    ifs.open("MeshSurf.dat");
    vector<vector<int>> surface;
    while(getline(ifs,str)){
        istringstream ss2(str);
        vector<int> tmp_surface;
        for(int j=0; j<3; j++){
          getline(ss2, str, ' ');
          tmp_surface.push_back(stoi(str)-1);
        }
        surface.push_back(tmp_surface);
    }

    cout << judge.size() << endl;
    cout << element.size() << endl;
    export_vtu("test.vtu",element,x,judge);
}