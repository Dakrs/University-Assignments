

#include "../headers/Figure.h"

void Figure::draw() {
    glPushMatrix();

    vector<Transformation*>::iterator it1;
    for(it1 = transformacoes->begin(); it1 != transformacoes->end(); it1++){
        (*it1)->perform();
    }

    vector<Material*>::iterator it2;
    for(it2 = materials->begin(); it2 != materials->end(); it2++){
        (*it2)->perform();
    }


    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glVertexPointer(3,GL_FLOAT,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
    glNormalPointer(GL_FLOAT,0,0);

    if (errors[2] == 0) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    glDrawArrays(GL_TRIANGLES, 0, n_vertex * 3);

    if (errors[2] == 0) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glPopMatrix();
}

Figure::Figure(vector<Point*> * v,vector<Transformation*> &trans,vector<Point*> * normal,vector<Material*> *  mat,vector<Point *> * texturepoint,const char * texturefile){
    materials = mat;

    transformacoes = new vector<Transformation*>();
    vector<Transformation*>::iterator it;
    for(it = trans.begin(); it != trans.end(); it++){
        transformacoes->push_back((*it)->clone());
    }


    n_vertex = v->size();
	float * array_vertexs = (float *) malloc(sizeof(float) * 3 * n_vertex);
	float * array_normal = (float *) malloc(sizeof(float) * 3 * n_vertex);
	vector<float> textures;

    vector<Point*>::iterator point_it;
    int vertex = 0;
    for(point_it = v->begin(); point_it != v->end(); point_it++){
        Point * p = (*point_it);
        array_vertexs[vertex*3 + 0] = p->getX();
        array_vertexs[vertex*3 + 1] = p->getY();
        array_vertexs[vertex*3 + 2] = p->getZ();

        vertex++;
    }
    vertex = 0;
    for(point_it = normal->begin(); point_it != normal->end(); point_it++){
        Point * p = (*point_it);
        array_normal[vertex*3 + 0] = p->getX();
        array_normal[vertex*3 + 1] = p->getY();
        array_normal[vertex*3 + 2] = p->getZ();

        vertex++;
    }
    vertex = 0;
    for(point_it = texturepoint->begin(); point_it != texturepoint->end(); point_it++){
        Point * p = (*point_it);
        textures.push_back(p->getX());
        textures.push_back(p->getY());

        vertex++;
    }

    cleanVector(v);
    cleanVector(normal);
    glGenBuffers(3,buffer);

    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex * 3,array_vertexs,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex * 3,array_normal,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(float), &(textures[0]), GL_STATIC_DRAW);

    free(array_vertexs);
    free(array_normal);
    //free(array_texture);

    if (texturefile != NULL && texturefile[0] != '\0') {
        loadImage(texturefile);
        errors[2] = 0;
    }
    else{
        errors[2] = 1;
    }
    cout << errors[2] << endl;
}

Figure::~Figure(){
    vector<Transformation*>::iterator it1;
    for(it1 = transformacoes->begin(); it1 != transformacoes->end(); it1++){
        delete((*it1));
    }
    delete(transformacoes);
}

void Figure::loadImage(const char * texturefile) {

    if (!texturefile)
        return;

        unsigned int t, tw, th;
        unsigned char *texData = NULL;

        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

        ilGenImages(1, &t);
        ilBindImage(t);
        ILboolean a = ilLoadImage((ILstring)texturefile);
        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();

        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

        //glGenerateMipmap(GL_TEXTURE_2D);
}


void cleanVector(vector<Point*> * limpar){
    vector<Point*>::iterator it;
    for(it = limpar->begin(); it != limpar->end(); it++){
        delete((*it));
    }
    delete(limpar);
}
