#include <stdlib.h>/*malloc free*/
#include <stdio.h>/*printf*/

#define MAX(X,Y) (X > Y ? X : Y)

static int s_count = 0;
/*-----------------FWD Declerations-------------------*/
typedef struct PT PT;
typedef struct Taxi Taxi;
typedef struct STaxi STaxi;
typedef struct Minibus Minibus;
/*             PT            */
void PTDisplay( PT *const self);
void PTDestroy(PT *const self);
int get_ID(PT *const self);
void PTInit(PT *const self);
void PTDealloc(PT *const self);
void PTCctor(PT *const self, const PT *const other);

/*             TAXI            */
void TaxiDestroy(Taxi *const self);
void TaxiDisplay(Taxi *const self);
Taxi *TaxiAlloc();
void TaxiInit(Taxi *const self);
void TaxiDealloc(Taxi *const self);
void TaxiCCtor (Taxi *const self, Taxi const *const other);
/*             STAXI            */
void STaxiDestroy(STaxi *const self);
void STaxiDisplay(STaxi *const self);
STaxi *STaxiAlloc();
void STaxiInit(STaxi *const self);
void STaxiDealloc(STaxi *const self);
void STaxiCCtor (STaxi *const self, STaxi const *const other);
/*             MINIBUS            */
void MinibusDisplay(Minibus * const self);
void MinibusWash(Minibus * const self, int min);
void MinibusDestroy(Minibus * const self);
Minibus *MinibusAlloc();
void MinibusInit(Minibus * const self);
void MinibusDealloc(Minibus * const self);
void MinibusCCtor (Minibus * const self, const Minibus *const other);

void Minibus_print_info(Minibus *m);
void int_print_info(int i, PT *placement);
void PT_print_info(PT *pt);
void empty_print_info();
void taxi_display(Taxi *s);
/*----------------------structs-------------------------------*/

typedef struct PTVTable 
{
    
    void (*display)(PT *const self);
    void (*Destroy)(PT *const self);
    void (*Dealloc)(PT *const self);

}PTVTable;

struct PT 
{
    void *vptr;
    int m_license_plate;

};

static PTVTable  publicT_Vtable = {PTDisplay, PTDestroy, PTDealloc};

typedef struct TaxiVTable 
{
    void (*display)(Taxi *const self);
    void (*Destroy)(Taxi *const self);
    void (*Dealloc)(Taxi *const self);

}TaxiVTable;

struct Taxi
{
    PT base;
};

static  TaxiVTable taxi_v_table = {TaxiDisplay, TaxiDestroy, TaxiDealloc};

typedef struct STaxiVTable 
{
    void (*display)(STaxi *const self);
    void (*Destroy)(STaxi *const self);
    void (*Dealloc)(STaxi *const self);
    
}STaxiVTable;

struct STaxi
{
    Taxi base; 
};

static  STaxiVTable staxi_v_table = {STaxiDisplay, STaxiDestroy, STaxiDealloc };

typedef struct MinibusVTable 
{
    void (*display)(Minibus *const self);
    void (*Destroy)(Minibus *const self);
    void (*Dealloc)(Minibus *const self);
    void (*wash)(Minibus *const self, int min);

}MinibusVTable;

struct Minibus
{
    PT base;
    int m_numSeats; 
};

static  MinibusVTable minibus_v_table = {MinibusDisplay, MinibusDestroy, 
                                            MinibusDealloc, MinibusWash};

/*--------------------allocs & inits & cctor---------------------------------*/

void PTInit(PT *const self)
{   
    ++s_count;
    self->m_license_plate = s_count;
    self->vptr = &publicT_Vtable;
    printf("PublicTransport::Ctor()%d\n" ,self->m_license_plate );
}

PT *PTAlloc()
{
    return malloc(sizeof(PT));
}

void PTCctor(PT *const self, const PT *other)
{
    ++s_count;
    self->vptr = &publicT_Vtable;
    self->m_license_plate = s_count;
    printf("PublicTransport::CCtor() %d\n",s_count);
}

Taxi *TaxiAlloc()
{
    
    return malloc(sizeof(Taxi));
}

void TaxiInit(Taxi * const self)
{
    PTInit(&(self->base));
    self->base.vptr = (void *)&taxi_v_table;
    printf( "Taxi::Ctor()\n");
}

void TaxiCCtor (Taxi *const self, const Taxi *other) 
{
    PTCctor(&(self->base), &(other->base));
    self->base.vptr = (void *)&taxi_v_table;
    printf("Taxi::CCtor()\n");
}

STaxi *STaxiAlloc()
{
    
    return malloc(sizeof(STaxi));
}

void STaxiInit(STaxi *const self)
{
    TaxiInit(&(self->base));
    self->base.base.vptr = (void *)&staxi_v_table;
    printf( "SpecialTaxi::Ctor()\n");
}

void STaxiCCtor (STaxi *const self, const STaxi *other) 
{
    self->base.base.vptr = (void *)&staxi_v_table;
    TaxiCCtor(&(self->base), &(other->base));
    printf("STaxi::CCtor()\n");
}

Minibus *MinibusAlloc()
{
    
    return malloc(sizeof(Minibus));
}

void MinibusInit(Minibus *const self)
{
    PTInit(&(self->base));
    self->base.vptr = (void *)&minibus_v_table;
    self->m_numSeats = 20;
    printf( "Minibus::Ctor()\n");
}

void MinibusCCtor (Minibus *const self, const Minibus *other) 
{
    PTCctor(&(self->base), &(other->base));
    self->base.vptr = &minibus_v_table;
    printf("Minibus::CCtor()\n");
}

/*--------------------Deallocs & Destroy---------------------------------*/

void PTDealloc(PT *const self)
{
    free(self);
}

void PTDestroy(PT *const self)
{
    --s_count;
    printf("PublicTransport::Dtor()%d\n" ,self-> m_license_plate);
    
}

void TaxiDestroy(Taxi * const self)
{
    printf("Taxi::Dtor()\n");
    PTDestroy(&(self->base));

}

void TaxiDealloc(Taxi * const self)
{
    free(self);
}

void STaxiDealloc(STaxi * const self)
{
    free(self);
}

void STaxiDestroy(STaxi * const self)
{
    printf("SpecialTaxi::Dtor()\n");
    TaxiDestroy(&(self->base));
}

void MinibusDestroy(Minibus * const self)
{
    printf( "Minibus::Dtor()\n");
    PTDestroy(&(self->base)); 
}

void MinibusDealloc(Minibus *const self)
{
    free(self);
}

/*-----------------------------------------------------*/
void TaxiDisplay(Taxi *const self)
{
    printf( "Taxi::display() ID:%d\n" , get_ID(&(self->base)));
}

static void print_count()
{
    printf("s_count: %d\n",s_count);
}

void Minibus_print_info(Minibus *m)
{
    ((MinibusVTable *)m->base.vptr)->wash(m, 3);
}

void int_print_info(int i, PT *placement)
{
    Minibus m;
    MinibusInit(&m);
    printf("print_info(int i)\n");
    ((MinibusVTable *)m.base.vptr)->display(&m);
    PTCctor(placement, (const PT *)&m);
    MinibusDestroy(&m);
}

void PT_print_info(PT *pt)
{
    ((PTVTable *)pt->vptr)->display(pt);
}

void empty_print_info()
{
    print_count();
}

void PTDisplay(PT *const self)
{
    printf("PublicTransport::display(): %d\n" ,self-> m_license_plate);
}

int get_ID(PT *const self)
{
    return self->m_license_plate;
}

void STaxiDisplay(STaxi *const self)
{
    printf( "STaxi::display() ID:%d\n" , get_ID(&(self->base.base)));
}

void taxi_display(Taxi *s)
{
    TaxiDisplay(s);
}

void MinibusDisplay(Minibus * const self)
{
    printf( "Minibus::display() ID:%d num seats:%d\n" , get_ID(&(self->base)),self->m_numSeats);
}

void MinibusWash(Minibus *const self, int min)
{
    printf("Minibus::wash(%d) ID:%d\n",min, get_ID(&(self->base)));
}


int main(int argc, char **argv, char **envp)
{
    Minibus m; 
    MinibusInit(&m);
    
    Minibus_print_info(&m);

    PT p ;
    int_print_info(3, &p);
    PTDisplay(&p);
    PTDestroy(&p);

    
    Minibus *minibus1 = MinibusAlloc();
    if(NULL == minibus1)
    {
        printf("alloc failed");
    }
    MinibusInit(minibus1);

    Taxi *taxi = TaxiAlloc();
    if(NULL == taxi)
    {
        printf("alloc failed");
    }
    TaxiInit(taxi);

    Minibus *minibus2 = MinibusAlloc();
    if(NULL == minibus2)
    {
        printf("alloc failed");
    }
    MinibusInit(minibus2);

    PT *array[] = {(PT *)minibus1, (PT *)taxi, (PT *)minibus2};

    for (int i = 0; i < 3; ++i) 
    {
       ((PTVTable *)array[i]->vptr)->display((array[i]));
    }

    for (int i = 0; i < 3; ++i) 
    {
        ((PTVTable *)array[i]->vptr)->Destroy((array[i]));
        ((PTVTable *)array[i]->vptr)->Dealloc((array[i]));
    }
    
    
    PT arr2[3];
    Minibus m1;
    MinibusInit(&m1);
    PTCctor(&(arr2[0]), &(m1.base));
    MinibusDestroy(&m1);

    Taxi t1;
    TaxiInit(&t1);
    PTCctor(&(arr2[1]), &(t1.base));
    TaxiDestroy(&t1);

 
    PTInit(&(arr2[2]));
    
    for (int i = 0; i < 3; ++i) 
    {
        PTDisplay(&(arr2[i]));
    }

    PT_print_info(&(arr2[0]));
    print_count();
    Minibus m2;
    MinibusInit(&m2);
    print_count();

    Minibus arr3[4];
    for(int i = 0; i<4 ;++i)
    {
        MinibusInit(&(arr3[i]));
    }
    Taxi *arr4 = malloc(sizeof(Taxi) * 4);
    for(int i = 0; i < 4 ;++i)
    {
        TaxiInit(&arr4[i]);
    }

    for(int i = 3; i >= 0 ;--i)
    {
        TaxiDestroy(&arr4[i]);
    }
    TaxiDealloc(arr4);

    printf("%d\n", MAX(1,2));
    printf("%d\n",MAX(1,(int)2.0f));
    STaxi st;
    STaxiInit(&st);
    Taxi tmp;
    TaxiCCtor(&tmp, (const Taxi *)&st);
    taxi_display(&tmp);
    TaxiDestroy(&tmp);
    STaxiDestroy(&st);

    for(int i = 3; i >=0 ;--i)
    {
        MinibusDestroy(&(arr3[i]));
    }

    MinibusDestroy(&m2);
    for (int i = 2; i >=0; --i) 
    {
        PTDestroy(&(arr2[i]));
    }

    MinibusDestroy(&m);
}


