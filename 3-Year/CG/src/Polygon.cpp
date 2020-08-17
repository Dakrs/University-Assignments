
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include "../headers/Polygon.h"

Point* Polygon::shpere_texture(float alpha,float beta) {
    float du = (1.0 / (2.0*M_PI)) * (-alpha) + 1.0;
    float dy = (1.0 / (M_PI)) * (beta + (M_PI / 2.0));

    return new Point(du,dy);
}

void Polygon::reverse_shpere(float radius,int slices,int stacks){
    int i=0;
    float alpha,d_alpha;
    float beta,d_beta;
    Point *p1,*p2,*p3;

    for(int k = 0; k < stacks ; k++){
        for(i=0; i < slices;i++){
            alpha = i*(2*M_PI / slices);
            beta = k*(M_PI / stacks) - (M_PI / 2);
            d_alpha =(2*M_PI / slices);
            d_beta = (M_PI / stacks);

            p1 = new Point(radius*cos(alpha)*cos(beta ),radius*sin(beta ),radius*sin(alpha)*cos(beta));
            p3 = new Point(radius*cos(alpha + d_alpha)*cos(beta) ,radius*sin(beta) ,radius*sin(alpha + d_alpha)*cos(beta));
            p2 = new Point(radius*cos(alpha)*cos(beta + d_beta),radius*sin(beta + d_beta),radius*sin(alpha)*cos(beta + d_beta));

            pontos->push_back(p1);
            normal->push_back(new Point(-cos(alpha)*cos(beta),-sin(beta),-sin(alpha)*cos(beta)));
            textures->push_back(shpere_texture(alpha,beta));

            pontos->push_back(p3);
            normal->push_back(new Point(-cos(alpha + d_alpha)*cos(beta),-sin(beta),-sin(alpha + d_alpha)*cos(beta)));
            textures->push_back(shpere_texture(alpha + d_alpha,beta));

            pontos->push_back(p2);
            normal->push_back(new Point(-cos(alpha)*cos(beta + d_beta),-sin(beta + d_beta),-sin(alpha)*cos(beta + d_beta)));
            textures->push_back(shpere_texture(alpha,beta + d_beta));

            p1 = new Point(radius*cos(alpha)*cos(beta + d_beta),radius*sin(beta + d_beta),radius*sin(alpha)*cos(beta + d_beta));
            p3 = new Point(radius*cos(alpha + d_alpha)*cos(beta) ,radius*sin(beta) ,radius*sin(alpha + d_alpha)*cos(beta));
            p2 = new Point(radius*cos(alpha + d_alpha)*cos(beta + d_beta) ,radius*sin(beta + d_beta) ,radius*sin(alpha + d_alpha)*cos(beta + d_beta));


            pontos->push_back(p1);
            normal->push_back(new Point(-cos(alpha)*cos(beta + d_beta),-sin(beta + d_beta),-sin(alpha)*cos(beta + d_beta)));
            textures->push_back(shpere_texture(alpha,beta + d_beta));

            pontos->push_back(p3);
            normal->push_back(new Point(-cos(alpha + d_alpha)*cos(beta),-sin(beta),-sin(alpha + d_alpha)*cos(beta)));
            textures->push_back(shpere_texture(alpha + d_alpha,beta));

            pontos->push_back(p2);
            normal->push_back(new Point(-cos(alpha + d_alpha)*cos(beta + d_beta),-sin(beta + d_beta),-sin(alpha + d_alpha)*cos(beta + d_beta)));
            textures->push_back(shpere_texture(alpha + d_alpha,beta + d_beta));
            
        }
    }
}

void Polygon::shpere_vertex(float radius,int slices,int stacks){
    int i=0;
    float alpha,d_alpha;
    float beta,d_beta;
    Point *p1,*p2,*p3;

    for(int k = 0; k < stacks ; k++){
        for(i=0; i < slices;i++){
            alpha = i*(2*M_PI / slices);
            beta = k*(M_PI / stacks) - (M_PI / 2);
            d_alpha =(2*M_PI / slices);
            d_beta = (M_PI / stacks);

            p1 = new Point(radius*cos(alpha)*cos(beta ),radius*sin(beta ),radius*sin(alpha)*cos(beta));
            p2 = new Point(radius*cos(alpha)*cos(beta + d_beta),radius*sin(beta + d_beta),radius*sin(alpha)*cos(beta + d_beta));
            p3 = new Point(radius*cos(alpha + d_alpha)*cos(beta) ,radius*sin(beta) ,radius*sin(alpha + d_alpha)*cos(beta));

            pontos->push_back(p1);
            normal->push_back(new Point(cos(alpha)*cos(beta),sin(beta),sin(alpha)*cos(beta)));
            textures->push_back(shpere_texture(alpha,beta));

            pontos->push_back(p2);
            normal->push_back(new Point(cos(alpha)*cos(beta + d_beta),sin(beta + d_beta),sin(alpha)*cos(beta + d_beta)));
            textures->push_back(shpere_texture(alpha,beta + d_beta));



            pontos->push_back(p3);
            normal->push_back(new Point(cos(alpha + d_alpha)*cos(beta),sin(beta),sin(alpha + d_alpha)*cos(beta)));
            textures->push_back(shpere_texture(alpha + d_alpha,beta));



            p1 = new Point(radius*cos(alpha)*cos(beta + d_beta),radius*sin(beta + d_beta),radius*sin(alpha)*cos(beta + d_beta));
            p2 = new Point(radius*cos(alpha + d_alpha)*cos(beta + d_beta) ,radius*sin(beta + d_beta) ,radius*sin(alpha + d_alpha)*cos(beta + d_beta));
            p3 = new Point(radius*cos(alpha + d_alpha)*cos(beta) ,radius*sin(beta) ,radius*sin(alpha + d_alpha)*cos(beta));

            pontos->push_back(p1);
            normal->push_back(new Point(cos(alpha)*cos(beta + d_beta),sin(beta + d_beta),sin(alpha)*cos(beta + d_beta)));
            textures->push_back(shpere_texture(alpha,beta + d_beta));



            pontos->push_back(p2);
            normal->push_back(new Point(cos(alpha + d_alpha)*cos(beta + d_beta),sin(beta + d_beta),sin(alpha + d_alpha)*cos(beta + d_beta)));
            textures->push_back(shpere_texture(alpha + d_alpha,beta + d_beta));


            pontos->push_back(p3);
            normal->push_back(new Point(cos(alpha + d_alpha)*cos(beta),sin(beta),sin(alpha + d_alpha)*cos(beta)));
            textures->push_back(shpere_texture(alpha + d_alpha,beta));
        }
    }
}

Polygon::Polygon(){
    pontos = new vector<Point*>();
    normal = new vector<Point*>();
    textures = new vector<Point*>();
}

Point * Polygon::getNormalCone(float edge [3], Point * p, float tangAlpha){
    float point_aux[3];

    p->get(point_aux);
    float distance = dist_2_P(edge,point_aux);

    float height = quadratic_equation(distance*tangAlpha,point_aux);

    float res [3];
    float point_at_aixis [3] = {0.0,height,0.0};
    vector_between_normalized(point_at_aixis,point_aux,res);
    return new Point(res[0],res[1],res[2]);
}

void Polygon::cone_vertex(float radius,float height,int slices,int stacks){
    float angle, height_act = 0, height_next = 0, radius_act = 0, radius_next = 0,d_alpha;
    int k = 0,i;
    float tangAlpha = radius / height;
    float edge [3] = {0.0, height, 0.0};

    Point *p1,*p2,*p3;

    //base
    for(i = 0; i < slices; i++){
        angle = i * ((2*M_PI) / slices);
        p1 = new Point(radius*cos(angle + ((2*M_PI) / slices)),0,radius*sin(angle + ((2*M_PI) / slices)));
        p2 = new Point(0,0,0);
        p3 = new Point(radius*cos(angle),0,radius*sin(angle));

        pontos->push_back(p1);
        normal->push_back(new Point(0.0,-1.0,0.0));
        pontos->push_back(p2);
        normal->push_back(new Point(0.0,-1.0,0.0));
        pontos->push_back(p3);
        normal->push_back(new Point(0.0,-1.0,0.0));
    }

    for(i = 0; i < slices; i++){
        for(k = 0; k < stacks - 1; k++){
            height_act = k * (height / stacks);
            height_next = (k+1) * (height / stacks);
            radius_act = (radius * (height - height_act)) / height;
            radius_next = (radius * (height - height_next)) / height;
            angle = i * ((2*M_PI) / slices);

            p1 = new Point(radius_act*cos(angle + ((2*M_PI) / slices)),height_act,radius_act*sin(angle + ((2*M_PI) / slices)));
            p2 = new Point(radius_act*cos(angle),height_act,radius_act*sin(angle));
            p3 = new Point(radius_next*cos(angle),height_next,radius_next*sin(angle));

            pontos->push_back(p1);
            normal->push_back(getNormalCone(edge,p1,tangAlpha));

            pontos->push_back(p2);
            normal->push_back(getNormalCone(edge,p2,tangAlpha));

            pontos->push_back(p3);
            normal->push_back(getNormalCone(edge,p3,tangAlpha));


            p1 = new Point(radius_next*cos(angle),height_next,radius_next*sin(angle));
            p2 = new Point(radius_next*cos(angle + ((2*M_PI) / slices)),height_next,radius_next*sin(angle + ((2*M_PI) / slices)));
            p3 = new Point(radius_act*cos(angle + ((2*M_PI) / slices)),height_act,radius_act*sin(angle + ((2*M_PI) / slices)));

            pontos->push_back(p1);
            normal->push_back(getNormalCone(edge,p1,tangAlpha));

            pontos->push_back(p2);
            normal->push_back(getNormalCone(edge,p2,tangAlpha));

            pontos->push_back(p3);
            normal->push_back(getNormalCone(edge,p3,tangAlpha));

        }
    }

    for(i = 0; i < slices; i++){
        height_act = k * (height / stacks);
        radius_act = (radius * (height - height_act)) / height;
        angle = i * ((2*M_PI) / slices);

        p1 = new Point(0,height,0);
        p2 = new Point(radius_act*cos(angle + ((2*M_PI) / slices)),height_act,radius_act*sin(angle + ((2*M_PI) / slices)));
        p3 = new Point(radius_act*cos(angle),height_act,radius_act*sin(angle));

        pontos->push_back(p1);
        normal->push_back(new Point(0,1,0));
        pontos->push_back(p2);
        normal->push_back(getNormalCone(edge,p2,tangAlpha));
        pontos->push_back(p3);
        normal->push_back(getNormalCone(edge,p3,tangAlpha));
    }
}

void Polygon::plane_vertex(float size) {
    Point *p1,*p2,*p3;

    p1 = new Point(size,0,size);
    p2 = new Point(size,0,-size);
    p3 = new Point(-size,0,-size);

    pontos->push_back(p1);
    normal->push_back(new Point(0,1,0));
    textures->push_back(new Point(1,1));

    pontos->push_back(p2);
    normal->push_back(new Point(0,1,0));
    textures->push_back(new Point(1,-1));


    pontos->push_back(p3);
    normal->push_back(new Point(0,1,0));
    textures->push_back(new Point(-1,-1));



    p1 = new Point(size,0,size);
    p2 = new Point(-size,0,-size);
    p3 = new Point(-size,0,size);

    pontos->push_back(p1);
    normal->push_back(new Point(0,1,0));
    textures->push_back(new Point(1,1));


    pontos->push_back(p2);
    normal->push_back(new Point(0,1,0));
    textures->push_back(new Point(-1,-1));

    pontos->push_back(p3);
    normal->push_back(new Point(0,1,0));
    textures->push_back(new Point(-1,1));

}

Point * Polygon::torus_texture(int slices,int stacks,int i,int j){
    float dx = 1.0 / stacks;
    float dy = 1.0/  slices;

    return new Point((-i*dx) + 1.0,j*dy);
}

void Polygon::torus_vertex(float radius_outside,float radius_inside,int slices,int stacks){
    float teta, fi,d_teta,d_fi;
    Point *p1,*p2,*p3;

    for(int i = 0; i < stacks ; i++){
        for(int j = 0; j < slices ; j++){
            teta = j * (2 * M_PI) / slices;
            fi = i * (2 * M_PI) / stacks;
            d_teta = (2 * M_PI) / slices;
            d_fi = (2 * M_PI) / stacks;


            p1 = new Point((radius_outside + radius_inside*cos(teta))*cos(fi),radius_inside*sin(teta),(radius_outside + radius_inside*cos(teta))*sin(fi));
            p2 = new Point((radius_outside + radius_inside*cos(teta + d_teta))*cos(fi),radius_inside*sin(teta + d_teta),(radius_outside + radius_inside*cos(teta + d_teta))*sin(fi));
            p3 = new Point((radius_outside + radius_inside*cos(teta))*cos(fi + d_fi),radius_inside*sin(teta),(radius_outside + radius_inside*cos(teta))*sin(fi + d_fi));

            pontos->push_back(p1);
            normal->push_back(new Point(cos(teta)*cos(fi),sin(teta),cos(teta)*sin(fi)));
            textures->push_back(torus_texture(slices,stacks,i,j));

            pontos->push_back(p2);
            normal->push_back(new Point(cos(teta + d_teta)*cos(fi),sin(teta + d_teta),cos(teta + d_teta)*sin(fi)));
            textures->push_back(torus_texture(slices,stacks,i,j+1));


            pontos->push_back(p3);
            normal->push_back(new Point(cos(teta)*cos(fi + d_fi),sin(teta),cos(teta)*sin(fi + d_fi)));
            textures->push_back(torus_texture(slices,stacks,i+1,j));


            p1 = new Point((radius_outside + radius_inside*cos(teta + d_teta))*cos(fi),radius_inside*sin(teta + d_teta),(radius_outside + radius_inside*cos(teta + d_teta))*sin(fi));
            p2 = new Point((radius_outside + radius_inside*cos(teta + d_teta))*cos(fi + d_fi),radius_inside*sin(teta + d_teta),(radius_outside + radius_inside*cos(teta + d_teta))*sin(fi + d_fi));
            p3 = new Point((radius_outside + radius_inside*cos(teta))*cos(fi + d_fi),radius_inside*sin(teta),(radius_outside + radius_inside*cos(teta))*sin(fi + d_fi));

            pontos->push_back(p1);
            normal->push_back(new Point(cos(teta + d_teta)*cos(fi),sin(teta + d_teta),cos(teta + d_teta)*sin(fi)));
            textures->push_back(torus_texture(slices,stacks,i,j+1));


            pontos->push_back(p2);
            normal->push_back(new Point(cos(teta + d_teta)*cos(fi + d_fi),sin(teta + d_teta),cos(teta + d_teta)*sin(fi + d_fi)));
            textures->push_back(torus_texture(slices,stacks,i+1,j+1));

            pontos->push_back(p3);
            normal->push_back(new Point(cos(teta)*cos(fi + d_fi),sin(teta),cos(teta)*sin(fi + d_fi)));
            textures->push_back(torus_texture(slices,stacks,i+1,j));

        }
    }
}

Point * Polygon::box_textures(float a, float b, float max_a, float max_b){
    float x = (a + max_a / 2.0) / max_a;
    float y = (b + max_b / 2.0) / max_b;

    return new Point(y,x);
}

void Polygon::box_vertex(float x,float y,float z,int partitions){
    float dx,dy,dz,sX,sY,sZ;

    dx = x / partitions;
    dy = y / partitions;
    dz = z / partitions;

    sX = -x/2;
    sY = -y/2;
    sZ = -z/2;

    int i,j;
    for(i = 0; i < partitions; i++){
        for(j = 0; j < partitions; j++){
            //Face Frente
            pontos->push_back(new Point(x/2,i*dy + sY,j*dz + sZ));
            normal->push_back(new Point(1,0,0));
            textures->push_back(box_textures(i*dy + sY,j*dz + sZ,y,z));

            pontos->push_back(new Point(x/2,(i+1)*dy + sY,j*dz + sZ));
            normal->push_back(new Point(1,0,0));
            textures->push_back(box_textures((i+1)*dy + sY,j*dz + sZ,y,z));


            pontos->push_back(new Point(x/2,i*dy + sY,(j+1)*dz + sZ));
            normal->push_back(new Point(1,0,0));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dz + sZ,y,z));


            pontos->push_back(new Point(x/2,(i+1)*dy + sY,j*dz + sZ));
            normal->push_back(new Point(1,0,0));
            textures->push_back(box_textures((i+1)*dy + sY,j*dz + sZ,y,z));


            pontos->push_back(new Point(x/2,(i+1)*dy + sY,(j+1)*dz + sZ));
            normal->push_back(new Point(1,0,0));
            textures->push_back(box_textures((i+1)*dy + sY,(j+1)*dz + sZ,y,z));


            pontos->push_back(new Point(x/2,i*dy + sY,(j+1)*dz + sZ));
            normal->push_back(new Point(1,0,0));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dz + sZ,y,z));


            //Face trás
            pontos->push_back(new Point(-x/2,i*dy + sY,j*dz + sZ));
            normal->push_back(new Point(-1,0,0));
            textures->push_back(box_textures(i*dy + sY,j*dz + sZ,y,z));


            pontos->push_back(new Point(-x/2,i*dy + sY,(j+1)*dz + sZ));
            normal->push_back(new Point(-1,0,0));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dz + sZ,y,z));


            pontos->push_back(new Point(-x/2,(i+1)*dy + sY,j*dz + sZ));
            normal->push_back(new Point(-1,0,0));
            textures->push_back(box_textures((i+1)*dy + sY,j*dz + sZ,y,z));


            pontos->push_back(new Point(-x/2,i*dy + sY,(j+1)*dz + sZ));
            normal->push_back(new Point(-1,0,0));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dz + sZ,y,z));


            pontos->push_back(new Point(-x/2,(i+1)*dy + sY,(j+1)*dz + sZ));
            normal->push_back(new Point(-1,0,0));
            textures->push_back(box_textures((i+1)*dy + sY,(j+1)*dz + sZ,y,z));


            pontos->push_back(new Point(-x/2,(i+1)*dy + sY,j*dz + sZ));
            normal->push_back(new Point(-1,0,0));
            textures->push_back(box_textures((i+1)*dy + sY,j*dz + sZ,y,z));


            //Face lateral frente
            pontos->push_back(new Point(j*dx + sX,i*dy + sY,z/2));
            normal->push_back(new Point(0,0,1));
            textures->push_back(box_textures(i*dy + sY,j*dx + sX,y,x));


            pontos->push_back(new Point((j+1)*dx + sX,i*dy + sY,z/2));
            normal->push_back(new Point(0,0,1));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dx + sX,y,x));


            pontos->push_back(new Point(j*dx + sX,(i+1)*dy + sY,z/2));
            normal->push_back(new Point(0,0,1));
            textures->push_back(box_textures((i+1)*dy + sY,j*dx + sX,y,x));


            pontos->push_back(new Point((j+1)*dx + sX,i*dy + sY,z/2));
            normal->push_back(new Point(0,0,1));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dx + sX,y,x));


            pontos->push_back(new Point((j+1)*dx + sX,(i+1)*dy + sY,z/2));
            normal->push_back(new Point(0,0,1));
            textures->push_back(box_textures((i+1)*dy + sY,(j+1)*dx + sX,y,x));


            pontos->push_back(new Point(j*dx + sX,(i+1)*dy + sY,z/2));
            normal->push_back(new Point(0,0,1));
            textures->push_back(box_textures((i+1)*dy + sY,j*dx + sX,y,x));


            //Face lateral trás
            pontos->push_back(new Point((j+1)*dx + sX,i*dy + sY,-z/2));
            normal->push_back(new Point(0,0,-1));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dx + sX,y,x));


            pontos->push_back(new Point(j*dx + sX,i*dy + sY,-z/2));
            normal->push_back(new Point(0,0,-1));
            textures->push_back(box_textures(i*dy + sY,j*dx + sX,y,x));


            pontos->push_back(new Point(j*dx + sX,(i+1)*dy + sY,-z/2));
            normal->push_back(new Point(0,0,-1));
            textures->push_back(box_textures((i+1)*dy + sY,j*dx + sX,y,x));


            pontos->push_back(new Point(j*dx + sX,(i+1)*dy + sY,-z/2));
            normal->push_back(new Point(0,0,-1));
            textures->push_back(box_textures((i+1)*dy + sY,j*dx + sX,y,x));


            pontos->push_back(new Point((j+1)*dx + sX,(i+1)*dy + sY,-z/2));
            normal->push_back(new Point(0,0,-1));
            textures->push_back(box_textures((i+1)*dy + sY,(j+1)*dx + sX,y,x));


            pontos->push_back(new Point((j+1)*dx + sX,i*dy + sY,-z/2));
            normal->push_back(new Point(0,0,-1));
            textures->push_back(box_textures(i*dy + sY,(j+1)*dx + sX,y,x));


            //Face cima
            pontos->push_back(new Point(j*dx + sX,y/2,i*dz + sZ));
            normal->push_back(new Point(0,1,0));
            textures->push_back(box_textures(i*dz + sZ,j*dx + sX,z,x));


            pontos->push_back(new Point(j*dx + sX,y/2,(i+1)*dz + sZ));
            normal->push_back(new Point(0,1,0));
            textures->push_back(box_textures((i+1)*dz + sZ,j*dx + sX,z,x));


            pontos->push_back(new Point((j+1)*dx + sX,y/2,(i+1)*dz + sZ));
            normal->push_back(new Point(0,1,0));
            textures->push_back(box_textures((i+1)*dz + sZ,(j+1)*dx + sX,z,x));


            pontos->push_back(new Point((j+1)*dx + sX,y/2,i*dz + sZ));
            normal->push_back(new Point(0,1,0));
            textures->push_back(box_textures(i*dz + sZ,(j+1)*dx + sX,z,x));


            pontos->push_back(new Point(j*dx + sX,y/2,i*dz + sZ));
            normal->push_back(new Point(0,1,0));
            textures->push_back(box_textures(i*dz + sZ,j*dx + sX,z,x));


            pontos->push_back(new Point((j+1)*dx + sX,y/2,(i+1)*dz + sZ));
            normal->push_back(new Point(0,1,0));
            textures->push_back(box_textures((i+1)*dz + sZ,(j+1)*dx + sX,z,x));


            //Face baixo
            pontos->push_back(new Point((j+1)*dx + sX,-y/2,(i+1)*dz + sZ));
            normal->push_back(new Point(0,-1,0));
            textures->push_back(box_textures((i+1)*dz + sZ,(j+1)*dx + sX,z,x));


            pontos->push_back(new Point(j*dx + sX,-y/2,(i+1)*dz + sZ));
            normal->push_back(new Point(0,-1,0));
            textures->push_back(box_textures((i+1)*dz + sZ,j*dx + sX,z,x));


            pontos->push_back(new Point(j*dx + sX,-y/2,i*dz + sZ));
            normal->push_back(new Point(0,-1,0));
            textures->push_back(box_textures(i*dz + sZ,j*dx + sX,z,x));


            pontos->push_back(new Point(j*dx + sX,-y/2,i*dz + sZ));
            normal->push_back(new Point(0,-1,0));
            textures->push_back(box_textures(i*dz + sZ,j*dx + sX,z,x));


            pontos->push_back(new Point((j+1)*dx + sX,-y/2,i*dz + sZ));
            normal->push_back(new Point(0,-1,0));
            textures->push_back(box_textures(i*dz + sZ,(j+1)*dx + sX,z,x));


            pontos->push_back(new Point((j+1)*dx + sX,-y/2,(i+1)*dz + sZ));
            normal->push_back(new Point(0,-1,0));
            textures->push_back(box_textures((i+1)*dz + sZ,(j+1)*dx + sX,z,x));

        }
    }
}

Point * Polygon::cylinger_textures(float alpha, float height, float heightmax){
    float x = (- 1.0 / (2.0 * M_PI)) * alpha + 1.0;
    float y = (1.0 - 0.375) * ((height + heightmax / 2.0) / heightmax) + 0.375;

    return new Point(x,y);
}

void Polygon::cylinder_vertex(float radius, float height,int slices,int stacks){
    float alpha,beta;


    int i,j;

    for(j = 0; j <= stacks; j++){
        beta = (j * (height / stacks)) - height/2;
        for(i = 0; i < slices; i++){
            alpha = i * (2*M_PI) / slices;

            if (j == stacks){
                pontos->push_back(new Point(radius*cos(alpha),height/2,radius*sin(alpha)));
                normal->push_back(new Point(0,1,0));
                textures->push_back(new Point(0.4375 + 0.1875*cos(alpha),0.1875+ 0.1875*sin(alpha)));

                pontos->push_back(new Point(0,height/2,0));
                normal->push_back(new Point(0,1,0));
                textures->push_back(new Point(0.4375,0.1875));


                pontos->push_back(new Point(radius*cos(alpha + (2*M_PI) / slices),height/2,radius*sin(alpha + (2*M_PI) / slices)));
                normal->push_back(new Point(0,1,0));
                textures->push_back(new Point(0.4375 + 0.1875*cos(alpha + (2*M_PI) / slices),0.1875+ 0.1875*sin(alpha + (2*M_PI) / slices)));

            }
            else {
                if (j == 0){
                    pontos->push_back(new Point(0,-height/2,0));
                    normal->push_back(new Point(0,-1,0));
                    textures->push_back(new Point(0.8125,0.1875));

                    pontos->push_back(new Point(radius*cos(alpha),-height/2,radius*sin(alpha)));
                    normal->push_back(new Point(0,-1,0));
                    textures->push_back(new Point(0.8125 + 0.1875*cos(alpha),0.1875+ 0.1875*sin(alpha)));


                    pontos->push_back(new Point(radius*cos(alpha + (2*M_PI) / slices),-height/2,radius*sin(alpha + (2*M_PI) / slices)));
                    normal->push_back(new Point(0,-1,0));
                    textures->push_back(new Point(0.8125 + 0.1875*cos(alpha + (2*M_PI) / slices),0.1875+ 0.1875*sin(alpha + (2*M_PI) / slices)));

                }

                pontos->push_back(new Point(radius*cos(alpha),beta,radius*sin(alpha)));
                normal->push_back(new Point(cos(alpha),0,sin(alpha)));
                textures->push_back(cylinger_textures(alpha,beta,height));

                pontos->push_back(new Point(radius*cos(alpha),beta + (height / stacks),radius*sin(alpha)));
                normal->push_back(new Point(cos(alpha),0,sin(alpha)));
                textures->push_back(cylinger_textures(alpha,beta  + (height / stacks) ,height));


                pontos->push_back(new Point(radius*cos(alpha + (2*M_PI) / slices),beta,radius*sin(alpha + (2*M_PI) / slices)));
                normal->push_back(new Point(cos(alpha + (2*M_PI) / slices),0,sin(alpha + (2*M_PI) / slices)));
                textures->push_back(cylinger_textures(alpha + (2*M_PI) / slices,beta,height));



                pontos->push_back(new Point(radius*cos(alpha),beta + (height / stacks),radius*sin(alpha)));
                normal->push_back(new Point(cos(alpha),0,sin(alpha)));
                textures->push_back(cylinger_textures(alpha,beta + (height / stacks),height));


                pontos->push_back(new Point(radius*cos(alpha + (2*M_PI) / slices),beta + (height / stacks),radius*sin(alpha + (2*M_PI) / slices)));
                normal->push_back(new Point(cos(alpha + (2*M_PI) / slices),0,sin(alpha + (2*M_PI) / slices)));
                textures->push_back(cylinger_textures(alpha + (2*M_PI) / slices,beta + (height / stacks),height));


                pontos->push_back(new Point(radius*cos(alpha + (2*M_PI) / slices),beta,radius*sin(alpha + (2*M_PI) / slices)));
                normal->push_back(new Point(cos(alpha + (2*M_PI) / slices),0,sin(alpha + (2*M_PI) / slices)));
                textures->push_back(cylinger_textures(alpha + (2*M_PI) / slices,beta,height));
            }
        }
    }
}

vector<Point*> * Polygon::getPontos(){
    return pontos;
}

vector<Point*> * Polygon::getNormais(){
    return normal;
}

vector<Point*> * Polygon::getTextures() {
    return textures;
}

Polygon::Polygon(vector<Point*> * v){
    pontos = v;
}

Polygon::~Polygon(){
    delete(pontos);
    delete(normal);
}

Point * Polygon::brezierValue(float u, Point * p0,Point * p1,Point * p2,Point * p3){
    float b0 = (1.0 - u) * (1.0 - u) * (1.0 - u);
    float b1 = 3.0 * u * (1.0 - u) * (1.0 - u);
    float b2 = 3 * u * u * (1.0 - u);
    float b3 = u * u * u;

    float x = p0->getX()*b0 + p1->getX()*b1 + p2->getX()*b2 +  p3->getX()*b3;
    float y = p0->getY()*b0 + p1->getY()*b1 + p2->getY()*b2 +  p3->getY()*b3;
    float z = p0->getZ()*b0 + p1->getZ()*b1 + p2->getZ()*b2 +  p3->getZ()*b3;

    return new Point(x,y,z);
}


Point * Polygon::dUBezier(Patch * patch, float u, float v, float res [3])
{
    Point * P[4];
    Point * vCurve[4];
    for (int i = 0; i < 4; ++i) {
        P[0] = patch->pointAt(i);
        P[1] = patch->pointAt(4+i);
        P[2] = patch->pointAt(8+i);
        P[3] = patch->pointAt(12+i);
        vCurve[i] = brezierValue(v,P[0],P[1],P[2],P[3]);
    }

    float b0 = -3 * powf((1-u),2);
    float b1 = 3 * (3*powf(u,2) - 4*u + 1);
    float b2 = 3*(2*u - 3*powf(u,2));
    float b3 = 3 * u * u;

    res[0] = vCurve[0]->getX()*b0 + vCurve[1]->getX()*b1 + vCurve[2]->getX()*b2 +  vCurve[3]->getX()*b3;
    res[1] = vCurve[0]->getY()*b0 + vCurve[1]->getY()*b1 + vCurve[2]->getY()*b2 +  vCurve[3]->getY()*b3;
    res[2] = vCurve[0]->getZ()*b0 + vCurve[1]->getZ()*b1 + vCurve[2]->getZ()*b2 +  vCurve[3]->getZ()*b3;



    return new Point(res[0],res[1],res[2]);
}

/*
Point * Polygon::dUBezier(Patch * patch, float u, float v, float res [3])
{
    Point * P[4];
    Point * vCurve[4];
    for (int i = 0; i < 4; ++i) {
        P[0] = patch->pointAt(i);
        P[1] = patch->pointAt(4+i);
        P[2] = patch->pointAt(8+i);
        P[3] = patch->pointAt(12+i);
        vCurve[i] = brezierValue(v,P[0],P[1],P[2],P[3]);
    }

    float matrix [4][3];

    for(int i=0; i < 4 ; i++)
        vCurve[i]->get(matrix[i]);

    mulVect(-3 * (1 - u) * (1 - u),matrix[0]);
    mulVect((3 * (1 - u) * (1 - u) - 6 * u * (1 - u)),matrix[1]);
    mulVect((6 * u * (1 - u) - 3 * u * u),matrix[2]);
    mulVect((3 * u * u),matrix[3]);

    sumMatrix(matrix,res);

    return new Point(res[0],res[1],res[2]);
}*/


Point * Polygon::dVBezier(Patch * patch, float u, float v,float res [3])
{
    Point * uCurve[4];
    Point * P[4];

    for (int i = 0; i < 4; ++i) {
        P[0] = patch->pointAt(i);
        P[1] = patch->pointAt(i+1);
        P[2] = patch->pointAt(i+2);
        P[3] = patch->pointAt(i+3);
        uCurve[i] = brezierValue(u,P[0],P[1],P[2],P[3]);
    }

    float b0 = -3 * powf((1-v),2);
    float b1 = 3 * (3*powf(v,2) - 4*v + 1);
    float b2 = 3*(2*v - 3*powf(v,2));
    float b3 = 3 * v * v;

    res[0] = uCurve[0]->getX()*b0 + uCurve[1]->getX()*b1 + uCurve[2]->getX()*b2 +  uCurve[3]->getX()*b3;
    res[1] = uCurve[0]->getY()*b0 + uCurve[1]->getY()*b1 + uCurve[2]->getY()*b2 +  uCurve[3]->getY()*b3;
    res[2] = uCurve[0]->getZ()*b0 + uCurve[1]->getZ()*b1 + uCurve[2]->getZ()*b2 +  uCurve[3]->getZ()*b3;



    return new Point(res[0],res[1],res[2]);


}


/*
Point * Polygon::dVBezier(Patch * patch, float u, float v,float res [3])
{
    Point * uCurve[4];
    Point * P[4];

    for (int i = 0; i < 4; ++i) {
        P[0] = patch->pointAt(i);
        P[1] = patch->pointAt(i+1);
        P[2] = patch->pointAt(i+2);
        P[3] = patch->pointAt(i+3);
        uCurve[i] = brezierValue(u,P[0],P[1],P[2],P[3]);
    }

    float matrix [4][3];

    for(int i=0; i < 4 ; i++)
        uCurve[i]->get(matrix[i]);

    mulVect(-3 * (1 - v) * (1 - v),matrix[0]);
    mulVect((3 * (1 - v) * (1 - v) - 6 * v * (1 - v)),matrix[1]);
    mulVect((6 * v * (1 - v) - 3 * v * v),matrix[2]);
    mulVect((3 * v * v),matrix[3]);

    sumMatrix(matrix,res);

    return new Point(res[0],res[1],res[2]);
}*/


Point * Polygon::brezierPatch(float u, float v,Patch * patch){
    Point * u_points [4];
    Point * v_points [4];

    int j = 0, k = 0;

    for(int i = 0; i < 16; i++){
        Point * aux = patch->pointAt(i);
        u_points[j] = aux;
        j++;

        if (j % 4 == 0){
            v_points[k++] = brezierValue(u,u_points[0],u_points[1],u_points[2],u_points[3]);

            j = 0;
        }
    }

    Point * res = brezierValue(v,v_points[0],v_points[1],v_points[2],v_points[3]);

    return res;
}

void Polygon::brezierPoints(vector<Patch *> * patches, int tessellation){
    float divisons = 1.0 / tessellation,u,u2,v,v2;
    float res1 [3], res2 [3], res3[3];

    for(int k = 0; k < patches->size(); k++) {
        Patch * p = patches->at(k);

        for (int i = 0; i < tessellation; i++) {
            for (int j = 0; j < tessellation; j++) {

                u = j * divisons;
                v = i * divisons;
                u2 = (j+1) * divisons;
                v2 = (i+1) * divisons;

                Point * p1 = brezierPatch(u,v,p);
                Point * p2 = brezierPatch(u2,v,p);
                Point * p3 = brezierPatch(u,v2,p);
                Point * p4 = brezierPatch(u2,v2,p);



                pontos->push_back(p4);
                dUBezier(p,u2,v2,res1);
                dVBezier(p,u2,v2,res2);
                cross(res1,res2,res3);
                normalize(res3);
                normal->push_back(new Point(res3[0],res3[1],res3[2]));
                textures->push_back(new Point(u2,v2));

                pontos->push_back(p1);
                dUBezier(p,u,v,res1);
                dVBezier(p,u,v,res2);
                cross(res1,res2,res3);
                normalize(res3);
                normal->push_back(new Point(res3[0],res3[1],res3[2]));
                textures->push_back(new Point(u,v));

                pontos->push_back(p2);
                dUBezier(p,u2,v,res1);
                dVBezier(p,u2,v,res2);
                cross(res1,res2,res3);
                normalize(res3);
                normal->push_back(new Point(res3[0],res3[1],res3[2]));
                textures->push_back(new Point(u2,v));



                pontos->push_back(p4);
                dUBezier(p,u2,v2,res1);
                dVBezier(p,u2,v2,res2);
                cross(res1,res2,res3);
                normalize(res3);
                normal->push_back(new Point(res3[0],res3[1],res3[2]));
                textures->push_back(new Point(u2,v2));

                pontos->push_back(p3);
                dUBezier(p,u,v2,res1);
                dVBezier(p,u,v2,res2);
                cross(res1,res2,res3);
                normalize(res3);
                normal->push_back(new Point(res3[0],res3[1],res3[2]));
                textures->push_back(new Point(u,v2));

                pontos->push_back(p1);
                dUBezier(p,u,v,res1);
                dVBezier(p,u,v,res2);
                cross(res1,res2,res3);
                normalize(res3);
                normal->push_back(new Point(res3[0],res3[1],res3[2]));
                textures->push_back(new Point(u,v));

            }
        }
    }
}

void mycross(float *a, float *b,float * res){
    res[0] = (-a[2])*b[1] + a[1]*b[2];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = -a[1] * b[0] + a[0]*b[1];
}

float dist_2_P(float p1 [3],float p2 [3]){
    float x = p1[0] - p2[0];
    float y = p1[1] - p2[1];
    float z = p1[2] - p2[2];

    return sqrtf(x*x + y*y + z*z);
}

float quadratic_equation(float middle,float p[3]){
    float res[2];

    float aux = middle*middle - p[0]*p[0] - p[2]*p[2];

    float b = -2.0 * p[1];
    float a = 1.0;
    float c = p[1]*p[1] - aux;

    float discriminant = b*b - 4*a*c;

    if (discriminant > 0) {
        res[0] = (-b + sqrt(discriminant)) / (2*a);
        res[1] = (-b - sqrt(discriminant)) / (2*a);
    }
    else if (discriminant == 0) {res[0] = (-b + sqrt(discriminant)) / (2*a);}
    else { return  -1.0;}

    if (res[0] < p[1])
        return res[0];
    return res[1];
}

void vector_between_normalized(float p1[3],float p2[3],float res[3]){
    res[0] = p2[0] - p1[0];
    res[1] = p2[1] - p1[1];
    res[2] = p2[2] - p1[2];

    normalize(res);
}

void sumMatrix(float matrix [4][3],float res [3]){
    res[0] = 0.0;
    res[1] = 0.0;
    res[2] = 0.0;


    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 3 ; j++)
            res[j] += matrix[i][j];

}

void mulVect(float a,float res[3]){
    res[0] *= a;
    res[1] *= a;
    res[2] *= a;
}

bool loadOBJ(const char * path,vector <Point *> * out_vertices, vector<Point *> * out_uvs, vector <Point *> * out_normals){

    vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    vector< Point *> * temp_vertices = new vector<Point*>();
    vector< Point *> * temp_uvs = new vector<Point*>();
    vector< Point *> * temp_normals = new vector<Point*>();

    FILE * file = fopen( path, "r");
    if(!file){
        printf("Não consegui abrir o ficheiro\n");
        return false;
    }
    float x, y ,z;
    while(1) {
        char line[128];
        int res = fscanf(file, "%s", line);
        if (res == EOF)
            break;

        if (!strcmp(line, "v")) {
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            temp_vertices->push_back(new Point(x,y,z));
        } else if (!strcmp(line, "vt")) {
            fscanf(file, "%f %f\n", &x, &y);
            temp_uvs->push_back(new Point(x, y, 0));
        } else if (!strcmp(line, "vn")) {
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            temp_normals->push_back(new Point(x,y,z));
        } else if(!strcmp(line, "f")) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("%d\n", matches);
                printf("Error parsing faces, different number of matches\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }

    }

    for(unsigned int i = 0; i < vertexIndices.size(); i++){
        unsigned int vertexIndex = vertexIndices[i];
        out_vertices->push_back(new Point((*temp_vertices)[vertexIndex -1]->getX(), (*temp_vertices)[vertexIndex -1]->getY(), (*temp_vertices)[vertexIndex -1]->getZ()));
    }


    for(unsigned int i = 0; i < normalIndices.size(); i++){
        unsigned int normalIndex = normalIndices[i];
        out_normals->push_back(new Point((*temp_normals)[normalIndex -1]->getX(), (*temp_normals)[normalIndex -1]->getY(), (*temp_normals)[normalIndex -1]->getZ()));
    }

    for(unsigned int i = 0; i < uvIndices.size(); i++){
        unsigned int uvIndex = uvIndices[i];
        out_uvs->push_back(new Point((*temp_uvs)[uvIndex -1]->getX(), (*temp_uvs)[uvIndex -1]->getY(), (*temp_uvs)[uvIndex -1]->getZ()));
    }

    return true;
}