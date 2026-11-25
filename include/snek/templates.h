/** 
 * This header allows using generic types in C. 
 * The following example creates function 'sum' which accepts 'int' as its type:
 * int TEMPLATE(sum,int)(int a, int b);
 */

#ifndef TEMPLATES_H_
#define TEMPLATES_H_ 

#define CAT(X,Y) X##_##Y
#define TEMPLATE(X,Y) CAT(X,Y)

#endif 
