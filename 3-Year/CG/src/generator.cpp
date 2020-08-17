

#include "../headers/Point.h"
#include "../headers/Polygon.h"
#include "../headers/Patch.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

vector<Patch *>* parseBezierPatch(string file_name){

	ifstream file(file_name);
	string line;

	if (file.fail()){
		cout << "Error while opening the file" << endl;
		return NULL;
	}

	getline(file,line);
	int  n_patches = stoi(line);
	vector<int> * patches_line =new vector<int>();
    vector<vector<int> > * patches_total = new vector<vector<int> >();
	for(int i = 0; i < n_patches; i++) {
        getline(file, line);
        vector<int> *patches_line = new vector<int>();

        for (int j = 0; j < 16; j++) {
            int split = line.find(",");
            string n_string = line.substr(0, split);
            patches_line->push_back(stoi(n_string));
            line.erase(0, split + 1);
        }
        patches_total->push_back(*patches_line);
        for (int j = 0; j < 16; j++) {
            cout << "--";
            cout << patches_line->at(j);
        }
        cout << "\n";

    }
    cout << "--";

	getline(file,line);
	int n_points = stoi(line),j;
	string token;
	float coords[3];
	vector<Point *> vertex_aux ;
    cout << "--";

	for(int i = 0; i < n_points; i++){
		getline(file,line);
		j = 0;
		while(j < 3){
			int split = line.find(",");
			string n_string = line.substr(0,split);
			coords[j++] = stof(n_string);
			line.erase(0,split+1);
		}

		vertex_aux.push_back(new Point(coords[0],coords[1],coords[2]));
	}

	vector<Patch *> * patches = new vector<Patch*>();
    vector<int> * patches_auxiliar =new vector<int>();

	for(int i = 0; i < n_patches; i++){
		Patch * p = new Patch();
		for(int k = 0; k < 16; k++){
            patches_auxiliar = &(patches_total->at(i));
			Point * point_aux = vertex_aux.at(patches_auxiliar->at(k));
			p->addControlPoint(
					new Point(
					point_aux->getX(),
					point_aux->getY(),
					point_aux->getZ())
					);
		}
		patches->push_back(p);
	}

	vector<Point*>::iterator it;
	for(it = vertex_aux.begin(); it != vertex_aux.end(); it++){
		delete((*it));
	}

	return patches;
}


void write_file(string file_name,vector<Point*> * vertices,vector<Point*> * normais,vector<Point *> * textures){

    system("mkdir -p ../files/ ");
    string path = "../files/" + file_name;

    ofstream outputFile;
    outputFile.open(path);

	vector<Point*>::iterator it,it2,it3;
    if (vertices->size() != normais->size()) {
		cout << "Alguma coisa não está certa" << endl;
		for(it = vertices->begin(); it != vertices->end(); it++){
			outputFile << (*(*it)).to_String() << endl;
		}
	}
    else{
		for(it = vertices->begin(), it2 = normais->begin(), it3 = textures->begin(); it != vertices->end() && it2 != normais->end() && it3 != textures->end(); it++,it2++,it3++){
			outputFile << (*(*it)).to_String() << (*(*it2)).to_String() << (*(*it3)).to_String()  << endl;
		}
    }

    outputFile.close();
}

void print_error(string s){
	cout << s << endl;
}

void printHelp(){
	cout << "#_____________________________ HELP _____________________________# " << endl;
	cout << "|                                                                |" << endl;
	cout << "|   Usage: ./generator {COMMAND} ... {OUTPUT FILE}               |" << endl;
	cout << "|   COMMANDS:                                                    |" << endl;
	cout << "| - plane                                                        |" << endl;
	cout << "|      Cria um plano paralelo a XZ                               |" << endl;
	cout << "|                                                                |" << endl;
	cout << "| - box [SIZE X] [SIZE Y] [SIZE Z] [DIVISIONS]                   |" << endl;
	cout << "|      Cria um paralelepipedo com as dimensões pretendidas.      |" << endl;
	cout << "|                                                                |" << endl;
	cout << "| - sphere [RADIUS] [SLICE] [STACK]                              |" << endl;
	cout << "|      Cria uma esfera com o raio, slices e stacks pretendidas.  |" << endl;
	cout << "|                                                                |" << endl;
	cout << "| - cone [RADIUS] [HEIGHT] [SLICE] [STACK]                       |" << endl;
	cout << "|      Cria um cone com o raio, altura, slices e stacks          |" << endl;
	cout << "|      pretendidas.                                              |" << endl;
	cout << "|                                                                |" << endl;
	cout << "| - torus [OUTER RADIUS] [INNER RADIUS] [RINGS DIVISIONS] [RINGS]|" << endl;
	cout << "|      Cria um torus com as dimensões pretendidas                |" << endl;
  	cout << "|                                                                |" << endl;
  	cout << "| - cylinder [RADIUS] [HEIGHT] [SLICE] [STACK]                   |" << endl;
	cout << "|      Cria um cilindro com o raio, altura, slices e stacks      |" << endl;
	cout << "|      pretendidas.                                              |" << endl;
  	cout << "|                                                                |" << endl;
	cout << "| - bezier [file with bezier patches] [tessellation]             |" << endl;
	cout << "|      Cria uma figura usando patches de bezier                  |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   OUTPUT FILE:                                                 |" << endl;
	cout << "| In the file section you can specify any file in which you wish |" << endl;
	cout << "| to save the coordinates generated with the previous commands.  |" << endl;
	cout << "|                                                                |" << endl;
	cout << "#________________________________________________________________#" << endl;
}

bool convertOBJ(string file_name){
    vector <Point *>  *vertices = new vector<Point*>;
    vector <Point *>  *normals  = new vector<Point*>;
    vector <Point *>  *uvs = new vector<Point*>;

    if (loadOBJ(file_name.c_str(), vertices,  uvs, normals)) {
        write_file(file_name + ".3d", vertices, normals, uvs);
        return true;
    }
    else
        return false;
}

int main(int argc, char **argv){

	Polygon f;
	int flag = 0,slices,stacks,opt;
	float radius,height,radius2;
	float x,y,z;



    if (argc == 3 && strcmp(argv[1],"plane") == 0){
		f.plane_vertex(2);
		flag = 1;
    }
    else if (argc == 6 && strcmp(argv[1],"sphere") == 0){
		radius = atof(argv[2]);
		slices = atoi(argv[3]);
		stacks = atoi(argv[4]);

		if (radius != 0 && slices != 0 && stacks != 0){
			f.shpere_vertex(radius,slices,stacks);
			flag = 1;
		}else print_error("Erro no input");
    }
    else if (argc == 7 && strcmp(argv[1],"cone") == 0){
		radius = atof(argv[2]);
		height = atof(argv[3]);
		slices = atoi(argv[4]);
		stacks = atoi(argv[5]);

		if (radius != 0 && slices != 0 && stacks != 0 && height != 0){
			f.cone_vertex(radius,height,slices,stacks);
			flag = 1;
		}else print_error("Erro no input");
    }
	else if ((argc == 7 || argc == 6) && strcmp(argv[1],"box") == 0){
		x = atof(argv[2]);
		y = atof(argv[3]);
		z = atof(argv[4]);

		if (argc == 7){
			opt = atoi(argv[5]);
		}
		else opt = 1;

		if (x != 0 && y != 0 && z != 0 && opt != 0){
			f.box_vertex(x,y,z,opt);
			flag = 1;
		}else print_error("Erro no input");
	}
	else if (argc == 7 && strcmp(argv[1],"torus") == 0){
		radius = atof(argv[2]);
		radius2 = atof(argv[3]);
		slices = atoi(argv[4]);
		stacks = atoi(argv[5]);

		if (radius != 0 && slices != 0 && stacks != 0 && radius2 != 0 && (radius >= radius2)){
			f.torus_vertex(radius,radius2,slices,stacks);
			flag = 1;
		}else print_error("Erro no input");
	}
	else if(argc == 7 && strcmp(argv[1],"cylinder")==0){
		radius = atof(argv[2]);
		height = atof(argv[3]);
		slices = atoi(argv[4]);
		stacks = atoi(argv[5]);

		if (radius != 0 && slices != 0 && stacks != 0 && height != 0){
			f.cylinder_vertex(radius,height,slices,stacks);
			flag = 1;
		}else print_error("Erro no input");
	}
	else if(argc == 5 && strcmp(argv[1],"bezier")==0){
		int tessellation = atoi(argv[3]);
		if (tessellation >= 1) {
			vector<Patch *> * n_array = parseBezierPatch(argv[2]);
			f.brezierPoints(n_array, tessellation);
			flag = 1;
		}
		else print_error("Erro no input");
	}
	else if (argc == 6 && strcmp(argv[1],"rsphere") == 0){
		radius = atof(argv[2]);
		slices = atoi(argv[3]);
		stacks = atoi(argv[4]);

		if (radius != 0 && slices != 0 && stacks != 0){
			f.reverse_shpere(radius,slices,stacks);
			flag = 1;
		}else print_error("Erro no input");
	}
    else if(argc == 3 && strcmp(argv[1],"obj")==0){
        string filepath = argv[2];
        if (!convertOBJ(argv[2])) {
            print_error("Erro ao processar ficheiro especificado, talvez o ficheiro não existe");
        }
    }
	else{
		printHelp();
	}

	if (flag){
		write_file(argv[argc-1],f.getPontos(),f.getNormais(),f.getTextures());
	}
}

