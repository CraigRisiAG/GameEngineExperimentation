// This file declares utility macros for managing Bullet objects in the Godot
// engine. It provides macros for creating and deleting Bullet objects, ensuring
// proper memory management and nullification of pointers after deletion.
#ifndef BULLET_UTILITIES_H
#define BULLET_UTILITIES_H

/**
        @author AndreaCatania
*/

#define bulletnew(cl) new cl

#define bulletdelete(cl)                                                       \
  {                                                                            \
    delete cl;                                                                 \
    cl = NULL;                                                                 \
  }
#endif
