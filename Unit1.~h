//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Graphics.hpp>
#include <Mask.hpp>
#include <MPlayer.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TButton *stBut;
        TButton *regisBut;
        TButton *helpBut;
        TPanel *p2Pan;
        TPanel *p1Pan;
        TImage *Image2;
        TLabel *de1Lab;
        TLabel *de2Lab;
        TImage *Image3;
        TButton *goBut;
        TImage *Image4;
        TImage *Image6;
        TLabel *t1Lab;
        TLabel *t2Lab;
        TPanel *roPan;
        TImage *roIma;
        TButton *nexBut;
        TLabel *Label5;
        TLabel *Label6;
        TButton *crBut;
        TButton *exBut;
        TTimer *crTim;
        TEdit *Edit2;
        TEdit *Edit1;
        TLabel *Label1;
        TTimer *shooting;
        TImage *Image5;
        TLabel *d1Lab;
        TLabel *d2Lab;
        TPanel *r2Pan;
        TImage *Image7;
        TImage *Image8;
        TImage *Image9;
        TImage *Image10;
        TLabel *overLab;
        TButton *agBut;
        TButton *bBut;
        TTimer *Timer2;
        void __fastcall stButClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall regisButClick(TObject *Sender);
        void __fastcall exButClick(TObject *Sender);
        void __fastcall crButClick(TObject *Sender);
        void __fastcall nexButClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall crTimTimer(TObject *Sender);
        void __fastcall shootingTimer(TObject *Sender);
        void __fastcall goButClick(TObject *Sender);
        void __fastcall agButClick(TObject *Sender);
        void __fastcall bButClick(TObject *Sender);
        void __fastcall helpButClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
class Account{
  private:
    int id,pas,num;
    static int anum;
  public:
    void save(Account a[]);
    void load(int &u,Account a[]);
    bool check(int i,int p);
    bool regis(int i,int p,int u);
};

int Account::anum(0);

class Role:public TImage{
  private:
    int lv,name,exp,rro,realhp,pow;
    int u,d,l,r;
    int move;
    int wy;
    static int rnum;
    TImage *all;
    void __fastcall walk(TObject *Sender);
  public:
    TLabel *hplook;
    int hp;
    Role(int a,int r);
    __fastcall ~Role();
    void save(Role *r[],int u);
    void load(Role *r[]);
    void crush(TPanel *p);
    void lvup();
    void hprecover();
    String deshow();
    String prepare();
    void keyset(int uu,int dd,int ll,int rr);
    int way();
    TTimer *wlk;
    int mypow();
    void dehp(int p);
};

int Role::rnum(0);

class gun:public TImage{
  private:
    int wid,len;
    int wy;
    int x,y;
    int pow;
    TImage *all;
    TTimer *sht;
    void __fastcall shoot(TObject *Sender);
  public:
    int mypow();
    bool die;
    void way(int w);
    gun(int t,int w,int l);
    __fastcall ~gun();
};

class wall:public TImage{
  private:
    TImage *all;
    int pic;
  public:
    int hp;
    wall();
    __fastcall ~wall();
};

class game:public TTimer{
  private:
    TLabel *st;
   TPanel *wp;
    int who;
    void __fastcall r1gtime(TObject *Sender);
  public:
    int time;
    game(TPanel *p,TLabel *l,int w);
    __fastcall ~game();
};
