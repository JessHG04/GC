/****************************************************************************************************

    CarGL (main.cpp)

    Grado en Ingenieria Multimedia.
    Practica de Graficos por Computador.
    OpenGL con Shaders.
  ------------------------------------------------------------------------------------------

    CarsGL (C) 2012-2016 - Juan Antonio Puchol Garcia (puchol@dccia.ua.es)

    Changelog:

    Version 1.0 (Noviembre 2012)
        - Version inicial para Code::Blocks 10.05

    Version 1.1 (Noviembre 2012)
        - Arreglado el bug de seleccion incorrecta debido al panel inferior de controles

    Version 1.2 (Octubre 2013 )
        - Adaptacion de la libreria GLUI a Code::Blocks 12.11 y 13.12

    Version 1.3 (Octubre 2014 )
        - Adaptacion de la libreria GLUI a Code::Blocks 13.12

    Version 2.0 (Octubre 2015 )
        - Adaptacion del codigo empleando Shaders (Modern OpenGL Graphics)

    Version 2.1 (Septiembre 2016 )
        - Modificacion de los scrollbars de las luces para manipular la posision (compatible con Code::Blocks 16.01)

    NOTA:   Para que esta aplicacion compile se necesita tener copiados los
            siguientes ficheros desde la carpeta "Para Copiar":

            glut32.dll  -> C:\Windows\system

            Si es un Windows de 32 bits:
            glew32s.lib -> C:\Program Files\CodeBlocks\MinGW\lib
            glew32.lib  -> C:\Program Files\CodeBlocks\MinGW\lib
            libglui32.a -> C:\Program Files\CodeBlocks\MinGW\lib
            glut32.lib  -> C:\Program Files\CodeBlocks\MinGW\lib

            glui.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL
            glut.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL
            glew.h      -> C:\Program Files\CodeBlocks\MinGW\include\GL

            y la carpeta completa
            glm         -> C:\Program Files\CodeBlocks\MinGW\include

            Si es un Windows de 64 bits:
            glew32s.lib -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            glew32.lib  -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            libglui32.a -> C:\Program Files (x86)\CodeBlocks\MinGW\lib
            glut32.lib  -> C:\Program Files (x86)\CodeBlocks\MinGW\lib

            glui.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL
            glut.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL
            glew.h      -> C:\Program Files (x86)\CodeBlocks\MinGW\include\GL

            y la carpeta completa
            glm         -> C:\Program Files (x86)\CodeBlocks\MinGW\include


            ACLARACION:
                Hay varias librerias libglui32.a en la carpeta "otros"
                - Renombrar libglui32.10.05.a a libglui32.a para usar Code::Blocks 10.05
                - Renombrar libglui32.12.11.a a libglui32.a para usar Code::Blocks 12.11
                - Renombrar libglui32.13.12.a a libglui32.a para usar Code::Blocks 13.12 o superior

****************************************************************************************************/

#include "Objects.h"
#include <GL\glui.h>
// Otras Variables Globales

/**************************************** myGlutKeyboard() **********/

void Keyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 27:
        case 'q':
            exit(0);
            break;

        case 'O':
        case 'o':
            //Como no se ve bien, desactivamos la vista Ortog para PrimeraPersona y VistaAerea
            if(escena.primeraPersona==0 && escena.vistaAerea==0)
            {
                escena.tipoVista=1;
                gui.Reshape(x,y);//Paralela
            }

            break;

        case 'P':
        case 'p':
            escena.tipoVista=0;
            gui.Reshape(x,y);//Perspectiva
            break;

        //Teclas para la rotacion

        case 'x':
        case 'X':
            escena.rotacionX=1;
            escena.rotacionY=0;
            escena.rotacionZ=0;
        break;

        case 'y':
        case 'Y':
            escena.rotacionX=0;
            escena.rotacionY=1;
            escena.rotacionZ=0;
        break;

        case 'z':
        case 'Z':
            escena.rotacionX=0;
            escena.rotacionY=0;
            escena.rotacionZ=1;
        break;
    }

    glutPostRedisplay();
}



/**************************************** mySpecialKey() *************/
//Teclas que no tienen codigo ASCI asociado
static void SpecialKey(int key, int x, int y)
{
    //Si hay algun objeto seleccionado
    if(escena.seleccion != 0){

        TPrimitiva *car = escena.GetCar(escena.seleccion);

        switch (key)
        {
            case GLUT_KEY_UP:   // El coche avanza
                if(car->v < 3){
                    car->v += VEL;
                }
                break;
            case GLUT_KEY_DOWN:   // El coche retrocede
                if(car->v > -3){
                    car->v -= VEL;
                }
                break;

            case GLUT_KEY_LEFT:   // Giramos ruedas
                if(car->anguloRuedas < 14)
                    car->anguloRuedas += 2;
                break;
            case GLUT_KEY_RIGHT:
                if(car->anguloRuedas > -14)
                    car->anguloRuedas -= 2;
                break;
        }

        glutPostRedisplay();

    }

}

/***************************************** myGlutMenu() ***********/

void Menu( int value )
{
  Keyboard( value, 0, 0 );
}

void Mouse(int button, int button_state, int x, int y )
{
    gui.Mouse(button, button_state, x, y);
}

void Render()
{
    escena.Render();
}

void Idle()
{
    gui.Idle();
}

void Reshape(int x, int y){
    gui.Reshape(x, y);
}

void Motion(int x, int y){
    gui.Motion(x, y);
}




/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    // Inicializa GLUT and crea la ventana principal
    glutInit(&argc, argv);
    //queremos utilizar RGBA, double buffer, zBuffer, stencilBuffer
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE );
    glutInitWindowPosition( 50, 25 );
    glutInitWindowSize( 1100, 710 );

    int main_window = glutCreateWindow( "CarGL V1.1 (2012)" );

    // Inicializa los valores de OpenGL para esta Aplicación
    escena.InitGL();
    gui.Init(main_window);

    //funciones para renderizar, escalar, teclas especiales...
    glutDisplayFunc( Render );
    GLUI_Master.set_glutReshapeFunc( Reshape );
    GLUI_Master.set_glutKeyboardFunc( Keyboard );
    GLUI_Master.set_glutSpecialFunc( SpecialKey );
    GLUI_Master.set_glutMouseFunc( Mouse );
    glutMotionFunc( Motion );

    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( Idle );

    // Crea los objetos
    /*
    TPrimitiva *suelo = new TPrimitiva(SUELO_ID, SUELO_ID);
    TPrimitiva *circuito = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);

	TPrimitiva *bandera = new TPrimitiva(BANDERA_ID, BANDERA_ID);
	TPrimitiva *arco = new TPrimitiva(ARCOS_ID, ARCOS_ID);
	TPrimitiva *barrera = new TPrimitiva(BARRERA_ID, BARRERA_ID); //Otra calle
	TPrimitiva *arboles1 = new TPrimitiva(ARBOLES1_ID, ARBOLES1_ID);
	TPrimitiva *arboles2 = new TPrimitiva(ARBOLES2_ID, ARBOLES2_ID);
	TPrimitiva *gradas = new TPrimitiva(GRADAS_ID, GRADAS_ID);*/

    TPrimitiva *car1 = new TPrimitiva(COCHE_ID*1, COCHE_ID);
	TPrimitiva *arbol = new TPrimitiva(1, ARBOL_ID);
    TPrimitiva *banco = new TPrimitiva(2,BANCO_ID);
    TPrimitiva *casa = new TPrimitiva(3, CASA_ID);
    //TPrimitiva *coche = new TPrimitiva(4, COCHE_ID);
    TPrimitiva *cohete = new TPrimitiva(5, COHETE_ID);
    TPrimitiva *farola = new TPrimitiva(6, FAROLA_ID);
    TPrimitiva *maceta = new TPrimitiva(7, MACETA_ID);
    //TPrimitiva *rueda = new TPrimitiva (8, RUEDA_ID);
    TPrimitiva *semaforo = new TPrimitiva(9, SEMAFORO_ID);
    TPrimitiva *senyal = new TPrimitiva(10, SENYAL_ID);
    TPrimitiva *suelo = new TPrimitiva(11, SUELO_ID);
    TPrimitiva *suelo2 = new TPrimitiva(12, SUELO2_ID);


//Añadimos objetos

    escena.AddObject(suelo);
    escena.AddObject(arbol);
    escena.AddObject(banco);
    escena.AddObject(casa);
    escena.AddObject(cohete);
    escena.AddObject(farola);
    escena.AddObject(maceta);
    escena.AddObject(semaforo);
    escena.AddObject(senyal);
    escena.AddObject(suelo2);
    /*escena.AddObject(circuito);
    escena.AddObject(bandera);
    escena.AddObject(arco);
    escena.AddObject(barrera);
    escena.AddObject(arboles1);
    escena.AddObject(arboles2);*/
    escena.AddCar(car1);
    escena.ultimoSelec=COCHE_ID;

    /**** Regular GLUT main loop ****/
    glutMainLoop();

    return EXIT_SUCCESS;
}

