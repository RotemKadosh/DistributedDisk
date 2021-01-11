#include <stdio.h> /*printf*/
#include <stdlib.h>/*malloc free*/
/*----------------decleration------------*/
typedef struct PublicTransport PT;

/*-----------------PT-----------------*/
static int s_count = 0;

struct PublicTransport
{
    void *vptr;
    int m_license_plate;
}; 

typedef struct PTVTable 
{
    void (*display)(PT *const self);
    void (*Destroy)(PT *const self);
}PTVTable;


PT *PTAlloc();
void PTInit(PT *const self);
void PTDestroy(PT *const self);
void PTCCtor(PT *const self, PT *const other_);
void PTDisplay(PT *const self);
static void print_count();
int get_ID(PT *const self);

static PTVTable pt_v_table = {PTDisplay, PTDestroy};

/*-----------------MINIBUS----------------*/
typedef struct Minibus
{
    PT base;
    int n_numSeats;
    
} Minibus;

typedef struct MinibusVTable 
{
    void (*display)(Minibus *const self);
    void (*Destroy)(Minibus *const self);
    void (*wash)(Minibus *const self, int min);

}MinibusVTable;


Minibus *MAlloc();
void MInit(Minibus *const self);
void MCCtor(Minibus *const self, Minibus *const other);
void MDisplay(Minibus *const self);
void MWash(Minibus *const self,int minutes);
void MDestroy(Minibus *const self);

static  MinibusVTable minibus_v_table = {MDisplay, MDestroy, MWash};

/*--------------TAXI------------------*/
typedef struct Taxi
{
    PT base;

}Taxi;

typedef struct TaxiVTable 
{
    void (*display)(Taxi *const self);
    void (*Destroy)(Taxi *const self);

}TaxiVTable;


Taxi *TAlloc();
void TInit(Taxi *const self);
void TCCtor (Taxi *const self, Taxi *const other);
void TDestroy(Taxi *const self);
void TDisplay(Taxi *const self);

static  TaxiVTable taxi_v_table = {TDisplay, TDestroy};

/*--------------STAXI------------------*/
typedef struct STaxi
{
    Taxi base;

}STaxi;

typedef struct STaxiVTable 
{
    void (*display)(STaxi *const self);
    void (*Destroy)(STaxi *const self);
    
}STaxiVTable;


STaxi *STAlloc();
void STInit(STaxi *const self);
void STCCtor(STaxi *const self, STaxi *const other);
void STDestroy(STaxi *const self);
void STDisplay(STaxi *const self);

static  STaxiVTable staxi_v_table = {STDisplay, STDestroy};

/*--------------PC------------------*/
typedef struct PublicConvoy
{
    PT base;
    PT *m_pt1;
    PT *m_pt2;
    Minibus m_m;
    Taxi m_t;
} PC;

typedef struct PConvoyVTable 
{
    void (*display)(PC *const self);
    void (*Destroy)(PC *const self);

}PCVTable;


void PCInit(PC *const self);
PC *PCAlloc();
void PCCCtor(PC *const self, PC *const other);
void PCDisplay(PC *const self);
void PCDestroy(PC *const self);

static PCVTable pc_v_table = {PCDisplay, PCDestroy};


/***************PT***********************/

PT *PTAlloc()
{
    return malloc(sizeof(PT));
}
void PTInit(PT *const self)
{   
    self->vptr = &pt_v_table;
    ++s_count;
    self->m_license_plate = s_count;
    
    printf("PublicTransport::Ctor()%d\n" ,self->m_license_plate);
}
void PTDestroy(PT *const self)
{
    self->vptr = &pt_v_table;
    --s_count;
    printf("PublicTransport::Dtor()%d\n" ,self->m_license_plate);
    
}
void PTCCtor(PT *const self, PT *const other_)
{
    (void)other_;
    self->vptr = &pt_v_table;
    self->m_license_plate= ++s_count;
    printf("PublicTransport::CCtor() %d\n", self->m_license_plate);
}
void PTDisplay(PT *const self)
{
    printf("PublicTransport::display(): %d\n" ,self->m_license_plate);
}
static void print_count()
{
    printf("s_count: %d\n",  s_count);
}
int get_ID(PT *const self)
{
    return self->m_license_plate;
}

/*-----------------Minibus-----------------------*/


Minibus *MAlloc()
{
    return malloc(sizeof(Minibus));
}
void MInit(Minibus *const self)
{
    PTInit(&(self->base));
    self->base.vptr = (void *)&minibus_v_table;
    self->n_numSeats = 20;
    printf( "Minibus::Ctor()\n"); 
}
void MCCtor(Minibus *const self, Minibus *const other)
{
    PTCCtor(&self->base, &other->base);
    self->base.vptr = &minibus_v_table;
    self->n_numSeats = other->n_numSeats;
    printf("Minibus::CCtor()\n");
}
void MDisplay(Minibus *const self)
{
    printf("Minibus::display() ID:%d num seats:%d\n",get_ID((PT *)self), self->n_numSeats);
}
void MWash(Minibus *const self,int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n", minutes , get_ID((PT *)self));
}
void MDestroy(Minibus *const self)
{
    self->base.vptr = &minibus_v_table;
    printf("Minibus::Dtor()\n");
    PTDestroy(&(self->base)); 
}

/*-----------------Taxi-----------------------*/

Taxi *TAlloc()
{
    return malloc(sizeof(Taxi));
}
void TInit(Taxi * const self)
{
    PTInit(&(self->base));
    self->base.vptr = (void *)&taxi_v_table;
    printf( "Taxi::Ctor()\n");
}
void TCCtor (Taxi *const self, Taxi *const other) 
{
    PTCCtor(&(self->base), &(other->base));
    self->base.vptr = (void *)&taxi_v_table;
    printf("Taxi::CCtor()\n");
}
void TDestroy(Taxi *const self)
{
    self->base.vptr = (void *)&taxi_v_table;
    printf("Taxi::Dtor()\n");
    PTDestroy(&(self->base));
}
void TDisplay(Taxi *const self)
{
    printf("Taxi::display() ID:%d\n", get_ID((PT *)self));
}

/*-----------------STaxi-----------------------*/

STaxi *STAlloc()
{
    return malloc(sizeof(STaxi));
}
void STInit(STaxi *const self)
{
    TInit(&(self->base));
    self->base.base.vptr = (void *)&staxi_v_table;
    printf("SpecialTaxi::Ctor()\n");
}
void STDestroy(STaxi *const self)
{
    self->base.base.vptr = (void *)&taxi_v_table;
    printf("SpecialTaxi::Dtor()\n");
    TDestroy(&(self->base));
}
void STDisplay(STaxi *const self)
{
    printf("SpecialTaxi::display() ID:%d\n", self->base.base.m_license_plate);
}
void STCCtor(STaxi *const self, STaxi *const other) 
{
    TCCtor(&(self->base), &(other->base));
    printf("SpecialTaxi::CCtor()\n");
}

/*-----------------PC-----------------------*/

PC *PCAlloc()
{
    return malloc(sizeof(PC));
}
void PCInit(PC *const self) 
{
    PTInit(&(self->base));
    self->base.vptr = (void *)&pc_v_table;
   
    self->m_pt1 = (PT *const)MAlloc();
    MInit((Minibus *const)self->m_pt1);

    self->m_pt2 = (PT *const)TAlloc();
    TInit((Taxi *const)self->m_pt2);

    MInit(&(self->m_m));
    TInit(&(self->m_t));
}
void PCDisplay(PC *const self)
{
    
   ((MinibusVTable *)self->m_pt1->vptr)->display((Minibus *const)self->m_pt1);
   ((TaxiVTable *)self->m_pt2->vptr)->display((Taxi *const)self->m_pt2);
   MDisplay(((Minibus *const)&(self->m_m)));
   TDisplay(((Taxi *const)&(self->m_t)));

}
void PCDestroy(PC *const self)
{
    self->base.vptr = (void *)&pc_v_table;
    ((MinibusVTable *)self->m_pt1->vptr)->Destroy((Minibus *const)self->m_pt1);
    free(self->m_pt1);
    ((TaxiVTable *)self->m_pt2->vptr)->Destroy((Taxi *const)self->m_pt2);
    free(self->m_pt2);

    TDestroy((Taxi *const)&(self->m_t));
    MDestroy((Minibus *const)&(self->m_m));
    PTDestroy(&self->base);
    
    
}
void PCCCtor(PC *const self, PC *const other)
{

    PTCCtor(&self->base, &other->base);
    self->base.vptr = (void *)&pc_v_table;
    self->m_pt1 = (PT *)MAlloc();
    MCCtor((Minibus *const)self->m_pt1,(Minibus *const)other->m_pt1);
    self->m_pt2 = (PT *)TAlloc();
    TCCtor((Taxi *const)self->m_pt2,(Taxi *const)other->m_pt2);

    MCCtor(&(self->m_m),&(other->m_m));
    TCCtor(&(self->m_t),&(other->m_t));
}

/*---------------------------------*/

void PT_print_info(PT *a)
{
    ((PTVTable *)(a->vptr))->display(a);
}

void empty_print_info()
{
    print_count();
}

void Minibus_print_info(Minibus *m)
{
    ((MinibusVTable *)m->base.vptr)->wash(m, 3);
}

void int_print_info(int i, PT *placement)
{
    Minibus ret;
    MInit(&ret);
    printf("print_info(int i)\n");
    MDisplay(&ret);
    PTCCtor(placement, &(ret.base));
    MDestroy(&ret);
    (void)i;
}
int Max(int a, int b)
{
    return (a > b) ? a : b;
}

void taxi_display(Taxi *s)
{
    TDisplay(s);
}

int main()
{
    Minibus m;
    PT placemet;
    PT *array[3];
    int i;
    PT arr2[3];
    Minibus mtmp;
    Taxi ttmp;
    Minibus m2;
    Minibus arr3[4];
    Taxi *arr4; 
    STaxi st;
    Taxi sttmp;
    PC *ts1;
    PC *ts2;

    MInit(&m);
    Minibus_print_info(&m);
    int_print_info(3, &placemet);
    PTDisplay(&placemet);
    PTDestroy(&placemet);
    
    array[0] = (PT *)MAlloc();
    MInit((Minibus *)array[0]);
    array[1] = (PT *)TAlloc();
    TInit((Taxi *)array[1]);
    array[2] = (PT *)MAlloc();
    MInit((Minibus *)array[2]);

    for (i = 0; i < 3; ++i) 
    {
       ((PTVTable *)array[i]->vptr)->display(array[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        ((PTVTable *)array[i]->vptr)->Destroy(array[i]);
        free(array[i]);
    }
    
    MInit(&mtmp);
    PTCCtor(&arr2[0], &(mtmp.base));
    MDestroy(&mtmp);

    TInit(&ttmp);
    PTCCtor(&arr2[1], &(ttmp.base));
    TDestroy(&ttmp);

    PTInit(&arr2[2]);
    for (i = 0; i < 3; ++i) 
    {
        PTDisplay(&arr2[i]);
    }
    PT_print_info(&arr2[0]);

    print_count();
    MInit(&m2);
    print_count();

    MInit(&arr3[0]); 
    MInit(&arr3[1]);
    MInit(&arr3[2]);
    MInit(&arr3[3]);

    arr4 = malloc(sizeof(Taxi) * 4);
    for(i = 0; i < 4 ; i++)
    {
        TInit(&arr4[i]);
    }
    for(i = 3; i >= 0 ; --i)
    {
        TDestroy(&arr4[i]);
    }
    free(arr4);

    printf("%d\n",Max(1,2));
    printf("%d\n",Max(1,2));

    STInit(&st);
    TCCtor(&sttmp,(Taxi *)&st);
    taxi_display(&sttmp);
    TDestroy(&sttmp);


    ts1 = PCAlloc();
    PCInit(ts1);
   
    ts2 = PCAlloc();
    PCCCtor(ts2, ts1);

    ((PCVTable *)ts1->base.vptr)->display(ts1);

    ((PCVTable *)ts2->base.vptr)->display(ts2);

    ((PCVTable *)ts1->base.vptr)->Destroy(ts1);
    free(ts1);
    ((PCVTable *)ts2->base.vptr)->display(ts2);
    ((PCVTable *)ts2->base.vptr)->Destroy(ts2);
    free(ts2);

    STDestroy(&st);

    MDestroy(&arr3[3]); 
    MDestroy(&arr3[2]);
    MDestroy(&arr3[1]);
    MDestroy(&arr3[0]);

    MDestroy(&m2);
    for (i = 2; i >= 0; --i) 
    {
        PTDestroy(&arr2[i]);
    }
    MDestroy(&m);
    return 0;

}   