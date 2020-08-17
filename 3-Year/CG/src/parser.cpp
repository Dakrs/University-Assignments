

#include "../headers/parser.h"
#include "../headers/Ambient.h"
#include "../headers/Diffuse.h"
#include "../headers/Emissive.h"
#include "../headers/Specular.h"

void build_figure(string path,vector<Transformation*> &trans,vector<Figure*> * figures,vector<Material*> *  materials,const char * texturefile ){

    string line;
    string delimiter = " ";
    float x,y,z;
    ifstream myfile(path);
    Figure * f;
    if (myfile.is_open())
    {
        vector<Point*> * v = new vector<Point*>();
        vector<Point*> * normal =  new vector<Point*>();
        vector<Point*> * texture = new vector<Point*>();

        while (getline(myfile,line)){
            size_t pos = 0;
            string token;

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            x = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            y = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            z = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            v->push_back(new Point(x,y,z));

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            x = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            y = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            z = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            normal->push_back(new Point(x,y,z));

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            x = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            pos = line.find(delimiter);
            token = line.substr(0, pos);
            y = atof(token.c_str());
            line.erase(0, pos + delimiter.length());

            texture->push_back(new Point(x,y));
        }
        f = new Figure(v,trans,normal,materials,texture,texturefile);
        figures->push_back(f);
        myfile.close();
    }
    else cout << "error2" << endl;
}

string mergePath(string path, string prog){

    if (prog.compare("") == 0)
        return prog;
    int i ;
    int flag = 0;
    for( i = path.length() - 1 ; i>=0 && !flag; i-- )
        if(path[i] == '/') flag = 1;

    std::string pathFull = path.substr (0,i+2);     // "think"

    string pathProg = pathFull + prog;

    cout << pathProg << endl;

    return pathProg;


}


vector<Material*> * parse_Objects_Materials(XMLElement * element){
    vector<Material*> *  materials = new vector<Material*>();
    for(element = element->FirstChildElement(); element; element = element->NextSiblingElement()){
        const char * type = element->Attribute("type");
        float colors[3] = {0.0,0.0,0.0};

        element->QueryFloatAttribute( "X", colors );
        element->QueryFloatAttribute( "Y", colors+1 );
        element->QueryFloatAttribute( "Z", colors+2 );

        if (strcmp(type,"Ambient") == 0)
            materials->push_back(new Ambient(colors));
        else if (strcmp(type,"Diffuse") == 0)
            materials->push_back(new Diffuse(colors));
        else if (strcmp(type,"Emissive") == 0)
            materials->push_back(new Emissive(colors));
        else if (strcmp(type,"Specular") == 0){
            float shinny = 0.0;
            element->QueryFloatAttribute( "Shinny", &shinny);
            materials->push_back(new Specular(colors,shinny));
        }
    }

    return materials;
}


void parseModels(string f_path,vector<Transformation*> &trans,XMLElement * element,vector<Figure*> * figures){
    for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {
        string figura = element->Attribute("file");
        const char * texturefile = element->Attribute("texture");

        string file2 = mergePath(f_path,figura);
        string file3 = mergePath(f_path,texturefile);

        cout << texturefile << endl;

        vector<Material*> *  materials = parse_Objects_Materials(element);

        build_figure(file2,trans,figures,materials,(const char *) file3.c_str());
    }

}

void parseLights(XMLElement *element,vector<Light*> * luzes){
    for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {
        const char * typestr = element->Attribute("type");

        int type = -1;
        float pos[3], dir[3], extra[2];

        element->QueryFloatAttribute( "X", pos );
        element->QueryFloatAttribute( "Y", pos+1 );
        element->QueryFloatAttribute( "Z", pos+2 );

        if (strcmp(typestr,"POINT") == 0){
            type = 0;
            luzes->push_back(new Light(type,pos));
        }
        else if (strcmp(typestr,"DIRECTIONAL") == 0) {
            type = 1;
            luzes->push_back(new Light(type,pos));
        }
        else if (strcmp(typestr,"SPOT") == 0) {
            type = 2;
            element->QueryFloatAttribute( "dirX", dir );
            element->QueryFloatAttribute( "dirY", dir+1 );
            element->QueryFloatAttribute( "dirZ", dir+2 );

            element->QueryFloatAttribute( "Angle", extra );
            element->QueryFloatAttribute( "EXPO", extra +1);

            luzes->push_back(new Light(type,pos,dir,extra));
        }
    }

};

vector<Figure*> * parseXML(string  f_path,vector<Light*> * luzes){
    XMLDocument xmlDoc;
    XMLElement *element;

    vector<Transformation*> transf;

    vector<Figure*> * figuras = new vector<Figure*>();

    if (!(xmlDoc.LoadFile(f_path.c_str()))) {

        element = xmlDoc.FirstChildElement();
        for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {
            const char * name = element->Name();
            if (strcmp(name,"group") == 0)
                parseGroup(f_path,transf,element,figuras);
            else if (strcmp(name,"lights") == 0)
                parseLights(element,luzes);
        }

    }
    else {
        cout << "Ficheiro não encontrado! " << endl;
    }

    return figuras;
}

void parseGroup(string f_path,vector<Transformation*> &trans,XMLElement * element,vector<Figure*> * figures){
    XMLElement * percorrer;

    float v[3];
    int v1[3];

    vector<Transformation*> clone;

    vector<Transformation*>::iterator it;
    for(it = trans.begin(); it != trans.end(); it++){
        clone.push_back((*it)->clone());
    }

    for(percorrer = element->FirstChildElement(); percorrer; percorrer = percorrer->NextSiblingElement()){
        string name = percorrer->Name();
        if (name.compare("translate") == 0 || name.compare("scale") == 0){
            v[0] = 0.0; v[1] = 0.0; v[2] = 0.0;
            int time = 0.0;
            percorrer->QueryIntAttribute( "Time", &time);
            percorrer->QueryFloatAttribute( "X", v );
            percorrer->QueryFloatAttribute( "Y", v+1 );
            percorrer->QueryFloatAttribute( "Z", v+2 );

            int type;
            if (name.compare("translate") == 0) {
                Translate * t = new Translate(time);
                if (time != 0.0){
                    parseTranslate(t,percorrer);
                }
                else {t->addPoint(new Point (v[0],v[1],v[2]));}
                clone.push_back(t);
            }
            else clone.push_back(new Scale(v));
        }
        else if (name.compare("rotate") == 0){

            float time = 0.0, angle = 0.0;
            v1[0] = 0; v1[1] = 0; v1[2] = 0;
            percorrer->QueryFloatAttribute( "Time", &time);
            percorrer->QueryFloatAttribute( "Angle", &angle);
            percorrer->QueryIntAttribute( "axisX", v1 );
            percorrer->QueryIntAttribute( "axisY", v1+1 );
            percorrer->QueryIntAttribute( "axisZ", v1+2 );


            clone.push_back(new Rotate(time, angle, v1));
        }
        else if (name.compare("models") == 0){
            parseModels(f_path,clone,percorrer,figures);
        }
        else if (name.compare("group") == 0){
            parseGroup(f_path,clone,percorrer,figures);
        }
    }

    for(it = clone.begin(); it != clone.end(); it++){
        delete (*it);
    }
}

void parseTranslate(Translate * t,XMLElement * element){
    float v[3];
    for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {
        element->QueryFloatAttribute( "X", v );
        element->QueryFloatAttribute( "Y", v+1 );
        element->QueryFloatAttribute( "Z", v+2 );

        t->addPoint(new Point(v[0],v[1],v[2]));
    }
}
