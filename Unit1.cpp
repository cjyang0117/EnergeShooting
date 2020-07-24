//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <fstream.h>

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
bool gamet=true,pa=true;
int usernum=0,cn=10,p1,p2;
int p1n=0,p2n=0;
int p1a1=0,p2a1=0,p1a2=0,p2a2=0,p1a3=0,p2a3=0,p1a4=0,p2a4=0,p1a5=0,p2a5=0;
int p1t=0,p2t=0;
int p1all1=100,p1all2=0,p1all3=0,p1all4=0,p1all5=0;
int p2all1=100,p2all2=0,p2all3=0,p2all4=0,p2all5=0;
bool start1=true,start2=true,r2start=true;
wall *w1[42],*w2[42];
gun *g1[50],*g2[50];
game *game1,*game2;
Account act[10];
Role *ro[10];

bool Account::check(int i,int p){
  if(i==0||p==0) return false;
  if(i==id&&p==pas){
    return true;
  }else{
    return false;
  }
}
bool Account::regis(int i,int p,int u){
  anum=u;

  if(anum>=11){
    ShowMessage("帳戶已滿");
    return false;
  }else{
    id=i; pas=p;
    return true;
  }
}
void Account::save(Account a[]){
  fstream fp;
  fp.open("ac.txt",ios::out);
  if(fp){
    fp<<anum<<"\n";
    for(int i=0;i<anum;i++){
      fp<<a[i].id<<" "<<a[i].pas<<"\n";
    }
  }
  fp.close();
}
void Account::load(int &u,Account a[]){
  fstream fp;
  fp.open("ac.txt",ios::in);
  if(fp){
    fp>>anum;
    u=anum;
    for(int i=0;i<anum;i++){
      fp>>a[i].id>>a[i].pas;
    }
  }
  fp.close();
}

Role::Role(int a,int r):TImage(Form1){
  realhp=5000;
  hp=realhp;
  lv=1;
  name=a;
  rro=r;
  exp=0;
  pow=10;
  move=0;
  int wy=1;
  pow=0;
  wy=0;

  Height=50;
  Width=40;
  Transparent=true;
  Stretch=true;

  hplook=new TLabel(Form1);
  hplook->AutoSize=false;
  hplook->Color=clRed;
  hplook->Width=40;
  hplook->Height=5;
}
__fastcall Role::~Role(){
  delete wlk;
  delete all;
  delete hplook;
}
void __fastcall Role::walk(TObject *Sender){
  if(GetAsyncKeyState(u)){
    wy=0;
    Top-=10;
    if(move==3) move=0;
    Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/3)*move,(all->Height/4)*3,(all->Width/3)*(move+1),(all->Height/4)*4));
    move++;
  }
  if(GetAsyncKeyState(d)){
    wy=1;
    Top+=10;
    if(move==3) move=0;
    Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/3)*move,0,(all->Width/3)*(move+1),(all->Height/4)*1));
    move++;
  }
  if(GetAsyncKeyState(l)){
    wy=2;
    Left-=10;
    if(move==3) move=0;
    Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/3)*move,all->Height/4,(all->Width/3)*(move+1),(all->Height/4)*2));
    move++;
  }
  if(GetAsyncKeyState(r)){
    wy=3;
    Left+=10;
    if(move==3) move=0;
    Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/3)*move,(all->Height/4)*2,(all->Width/3)*(move+1),(all->Height/4)*3));
    move++;
  }
  hplook->Left=Left;
  hplook->Top=Top-5;
  hplook->Parent=Parent;
  if(realhp!=hp) hplook->Width=hp/(realhp/40);
}
int Role::way(){
  return wy;
}
void Role::keyset(int uu,int dd,int ll,int rr){
  u=uu; d=dd; l=ll; r=rr;
}
void Role::crush(TPanel *p){
  if(Top<=0) Top=0;
  if(r2start){
    if(Top+Height>=(p->Height/9)*2) Top=(p->Height/9)*2-Height;
  }else{
    if(Top+Height>=(p->Height)) Top=(p->Height)-Height;
  }
  if(Left<=0) Left=0;
  if(Left+Width>=p->Width)  Left=p->Width-Width;
}
void Role::save(Role *r[],int u){
  rnum=u;
  fstream fp;
  fp.open("ro.txt",ios::out);
  if(fp){
    fp<<rnum<<"\n";
    for(int i=0;i<rnum;i++){
      fp<<r[i]->name<<" "<<r[i]->lv<<" "<<r[i]->realhp<<" "<<r[i]->rro<<" "<<r[i]->exp<<" "<<r[i]->pow<<"\n";
    }
  }
  fp.close();
}
void Role::load(Role *r[]){
  fstream fp;
  fp.open("ro.txt",ios::in);
  if(fp){
    fp>>rnum;
    for(int i=0;i<rnum;i++){
      if(i+1<rnum) r[i+1]=new Role(1,10);
      fp>>r[i]->name>>r[i]->lv>>r[i]->realhp>>r[i]->rro>>r[i]->exp>>r[i]->pow;
    }
  }
  fp.close();
}
String Role::deshow(){
  return "ID:\t"+IntToStr(name)+"\nLV:\t"+IntToStr(lv)+"\nHP:\t"+IntToStr(realhp)+"\nPOWER\t"+IntToStr(pow)+"\nEXP\t"+IntToStr(exp);
}
String Role::prepare(){
  all=new TImage(Form1);
  all->Picture->LoadFromFile("pic\\r"+AnsiString(rro)+".bmp");
  all->AutoSize=true;
  all->Transparent=true;

  Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect(all->Width/3,0,(all->Width/3)*2,all->Height/4));

  wlk=new TTimer(Form1);
  wlk->Interval=100;
  wlk->OnTimer=walk;

  return rro;
}
void Role::lvup(){
  exp+=25;
  if(exp==100){
    lv+=1;
    exp=0;
    realhp+=100;
    pow+=10;
  }
}
void Role::hprecover(){
  hp=realhp;
}
int Role::mypow(){
  return pow;
}
void Role::dehp(int p){
  hp-=p;
}
gun::gun(int t,int w,int l):TImage(Form1){
  x=0;
  y=0;
  die=true;
  wy=1;
  wid=w;
  len=l;
  Height=40;
  Width=40;
  if(t==0) pow=100;
  if(t==1) pow=200;
  if(t==2) pow=250;
  if(t==3) pow=300;
  if(t==4) pow=400;

  Transparent=true;
  Stretch=true;

  all=new TImage(Form1);
  all->AutoSize=true;
  all->Picture->LoadFromFile("pic\\e"+AnsiString(t)+".bmp");

  Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect(0,0,all->Width/w,all->Height/l));

  sht=new TTimer(Form1);
  sht->Interval=1;
  sht->OnTimer=shoot;
}
__fastcall gun::~gun(){
  delete all;
  delete sht;
}
int gun::mypow(){
  return pow;
}
void __fastcall gun::shoot(TObject *Sender){
  if(die){
    if(wy==0){
      Top-=5;
    }else{
      if(wy==1){
        Top+=5;
      }else{
        if(wy==2){
          Left-=5;
        }else{
          Left+=5;
        }
      }
    }
  }else{
    if(y<len){
      if(x==5){
        x=0;
        y++;
      }
      Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/wid)*x,(all->Height/len)*y,(all->Width/wid)*(x+1),(all->Height/len)*(y+1)));
      x++;
    }else{
      delete this;
    }
  }
}
void gun::way(int w){
  wy=w;
}
void Shooting(int typ,int wp,int &pa,int paa,int pt,gun *g[],int &pn,int t,int x,int y,TPanel *p){
  if(pa<paa&&pt==typ){
    g[pn]=new gun(t,x,y);
    g[pn]->Top=ro[wp]->Top+10;
    g[pn]->Left=ro[wp]->Left+5;
    g[pn]->Parent=p;
    int x=ro[wp]->way();
    g[pn]->way(x);
    pn++;
    pa++;
  }
}
wall::wall():TImage(Form1){
  Width=60;
  Height=50;
  Transparent=true;

  all=new TImage(Form1);
  all->AutoSize=true;
  all->Picture->LoadFromFile("pic\\w0.bmp");

  pic=random(3)+1;
  hp=pic*1000;

  int x=random(4);
  switch (pic){
    case 1:
      Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/3)*2,(all->Height/4)*x,(all->Width/3)*3,(all->Height/4)*(x+1)));
      break;
    case 2:
      Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/3)*1,(all->Height/4)*x,(all->Width/3)*2,(all->Height/4)*(x+1)));
      break;
    case 3:
      Canvas->CopyRect(Rect(0,0,Width,Height),all->Canvas,Rect((all->Width/3)*0,(all->Height/4)*x,(all->Width/3)*1,(all->Height/4)*(x+1)));
      break;
  }
}
__fastcall wall::~wall(){
  delete all;
}
game::game(TPanel *p,TLabel *l,int w):TTimer(Form1){
  time=50;
  Interval=1000;
  OnTimer=r1gtime;
  wp=p;
  st=l;
  who=w;
}
__fastcall game::~game(){

}
void __fastcall game::r1gtime(TObject *Sender){
  time-=1;
  st->Caption=AnsiString(time);
  if(time<=0){
    if(who==1){
      ro[p1]->wlk->Enabled=false;
      start1=false;
    }else{
      ro[p2]->wlk->Enabled=false;
      start2=false;
    }
    Enabled=false;
  }
}
void over(){
  pa=false;
  //Form1->MediaPlayer3->Stop();
  //Form1->MediaPlayer3->Rewind();
  //Form1->MediaPlayer4->Play();
  Form1->r2Pan->Height=500;
  Form1->r2Pan->Width=450;
  Form1->r2Pan->Top=60;
  Form1->Image7->Visible=false;
  Form1->Image9->Visible=true;
  Form1->Image9->Align=alClient;
  Form1->Image10->Visible=true;
  Form1->overLab->Visible=true;
  Form1->agBut->Visible=true;
  Form1->bBut->Visible=true;
  if(ro[p2]->hp<=0){
    Form1->Image10->Picture=Form1->Image5->Picture;
    ro[p1]->lvup();
    ro[p1]->save(ro,usernum);
    Form1->overLab->Caption=ro[p1]->deshow();
  }else{
    Form1->Image10->Picture=Form1->Image6->Picture;
    ro[p2]->lvup();
    ro[p2]->save(ro,usernum);
    Form1->overLab->Caption=ro[p2]->deshow();
  }
  ro[p1]->Parent=Form1->p1Pan;
  ro[p2]->Parent=Form1->p2Pan;
  ro[p1]->hprecover();
  ro[p2]->hprecover();
  ro[p1]->hplook->Width=40;
  ro[p2]->hplook->Width=40;
}
void fill(int &a1,int &a2,int &a3,int &a4,int &a5,int wp){
  int x=random(7);
  switch(x){
    case 0:
      a1+=100;
      if(wp==1){
        Form1->d1Lab->Caption=Form1->d1Lab->Caption+"Fire +100\n";
      }else{
        Form1->d2Lab->Caption=Form1->d2Lab->Caption+"Fire +100\n";
      }
      break;
    case 1:
      a2+=50;
      if(wp==1){
        Form1->d1Lab->Caption=Form1->d1Lab->Caption+"Dark +50\n";
      }else{
        Form1->d2Lab->Caption=Form1->d2Lab->Caption+"Dark +50\n";
      }
      break;
    case 2:
      a3+=40;
      if(wp==1){
        Form1->d1Lab->Caption=Form1->d1Lab->Caption+"Magic +40\n";
      }else{
        Form1->d2Lab->Caption=Form1->d2Lab->Caption+"Magic +40\n";
      }
      break;
    case 3:
      a4+=40;
      if(wp==1){
        Form1->d1Lab->Caption=Form1->d1Lab->Caption+"Water +40\n";
      }else{
        Form1->d2Lab->Caption=Form1->d2Lab->Caption+"Water +40\n";
      }
      break;
    case 4:
      a5+=20;
      if(wp==1){
        Form1->d1Lab->Caption=Form1->d1Lab->Caption+"Light +20\n";
      }else{
        Form1->d2Lab->Caption=Form1->d2Lab->Caption+"Light +20\n";
      }
      break;
    case 5:
      if(wp==1) game1->time+=20;
      if(wp==2) game2->time+=20;
      break;
    case 6:
      if(wp==1) game2->time-=20;
      if(wp==2) game1->time-=20;
      break;
  }
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::stButClick(TObject *Sender)
{
  int login[4];
  login[0]=StrToInt(InputBox("帳號       ","P1請輸入帳號        ","0"));
  login[1]=StrToInt(InputBox("密碼       ","P1請輸入密碼        ","0"));
  login[2]=StrToInt(InputBox("帳號       ","P2請輸入密碼        ","0"));
  login[3]=StrToInt(InputBox("密碼       ","P2請輸入密碼        ","0"));
  int i,j;
  for(i=0;i<10;i++){
    if(act[i].check(login[0],login[1])) break;
  }
  for(j=0;j<10;j++){
    if(act[j].check(login[2],login[3])) break;
  }
  if(i==10||j==10&&i!=j){
    ShowMessage("帳號密碼錯誤，或請勿輸入相同帳密兩次");
    return;
  }

  p1=i;  p2=j;
  ro[p1]->Parent=p1Pan;
  ro[p2]->Parent=p2Pan;
  Image5->Picture->LoadFromFile("pic\\c"+ro[p1]->prepare()+".bmp");
  Image6->Picture->LoadFromFile("pic\\c"+ro[p2]->prepare()+".bmp");
  de1Lab->Caption=ro[p1]->deshow();
  de2Lab->Caption=ro[p2]->deshow();
  ro[p1]->keyset(87,83,65,68);
  ro[p2]->keyset(38,40,37,39);
  crTim->Enabled=true;
  shooting->Enabled=true;

  Image1->Visible=false;
  stBut->Visible=false;
  regisBut->Visible=false;
  helpBut->Visible=false;

  Form1->Width=1100;
  Form1->Height=900;
  Form1->Position=poScreenCenter;
  Image2->Visible=true;
  Image2->Align=alClient;

  p1Pan->Visible=true;
  p2Pan->Visible=true;
  goBut->Visible=true;
  Image5->Visible=true;
  Image6->Visible=true;
  t1Lab->Visible=true;
  t2Lab->Visible=true;
  de1Lab->Visible=true;
  de2Lab->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  //MediaPlayer1->FileName="bgm\\01.wav";
  //MediaPlayer2->FileName="bgm\\02.wav";
  //MediaPlayer3->FileName="bgm\\03.wav";
  //MediaPlayer4->FileName="bgm\\04.wav";
  //MediaPlayer1->Play();

  act[0].load(usernum,act);

  if(usernum!=0){
    ro[0]=new Role(1,10);
    ro[0]->load(ro);
  }
  randomize();
  Form1->Width=341;
  Form1->Height=585;
  Image1->Align=alClient;
  Image3->Align=alClient;
  Image4->Align=alClient;
  p1Pan->DoubleBuffered=true;
  p2Pan->DoubleBuffered=true;
  r2Pan->DoubleBuffered=true;

  stBut->Width=160;
  stBut->Height=70;
  stBut->Left=Form1->Width/2-stBut->Width/2;
  stBut->Top=145;

  regisBut->Width=160;
  regisBut->Height=70;
  regisBut->Left=Form1->Width/2-stBut->Width/2;
  regisBut->Top=300;

  helpBut->Width=160;
  helpBut->Height=70;
  helpBut->Left=Form1->Width/2-stBut->Width/2;
  helpBut->Top=400;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::regisButClick(TObject *Sender)
{
  Form1->Width=950;
  Form1->Height=400;
  Form1->Position=poScreenCenter;
  roPan->Left=120;
  roPan->Top=25;
  roPan->Width=700;
  roPan->Height=288;
  roPan->Visible=true;
  Image1->Align=alNone;
  Image1->Visible=false;
  Image2->Align=alClient;
  Image2->Visible=true;
  stBut->Visible=false;
  regisBut->Visible=false;
  helpBut->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::exButClick(TObject *Sender)
{
  Form1->Width=341;
  Form1->Height=585;
  Form1->Position=poScreenCenter;
  roPan->Visible=false;
  Image1->Align=alClient;
  Image1->Visible=true;
  Image2->Visible=false;
  stBut->Visible=true;
  regisBut->Visible=true;
  helpBut->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::crButClick(TObject *Sender)
{
  int reg[2];
  reg[0]=Edit1->Text.ToInt();
  reg[1]=Edit2->Text.ToInt();
  Edit1->Text="0";
  Edit2->Text="0";
  if(reg[0]==0||reg[1]==0){
    ShowMessage("帳號密碼不可為零");
    return;
  }

  int i;
  for(i=0;i<10;i++){
    if(act[i].check(reg[0],reg[1])) break;
  }
  if(i<10){
    ShowMessage("帳號或密碼已被註冊");
    return;
  }else{
    if(act[usernum].regis(reg[0],reg[1],usernum+1)){
      act[usernum].save(act);
      ro[usernum]=new Role(reg[0],cn);
      ro[usernum]->save(ro,usernum+1);
      usernum++;
      ShowMessage("角色創立成功");
    }
  }
  Form1->Width=341;
  Form1->Height=585;
  Form1->Position=poScreenCenter;
  roPan->Visible=false;
  Image1->Align=alClient;
  Image1->Visible=true;
  Image2->Visible=false;
  stBut->Visible=true;
  regisBut->Visible=true;
  helpBut->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::nexButClick(TObject *Sender)
{
  cn++;
  if(cn==17) cn=10;
  roIma->Picture->LoadFromFile("pic\\c"+AnsiString(cn)+".bmp");
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormDestroy(TObject *Sender)
{
  for(int i=0;i<usernum;i++){
    delete ro[i];
    ro[i]=NULL;
  }

  for(int i=0;i<p1n;i++){
    delete g1[i];
    g1[i]=NULL;
  }
  for(int i=0;i<p2n;i++){
    delete g2[i];
    g2[i]=NULL;
  }

  for(int i=0;i<42;i++){
    if(w1[i]!=NULL){
      delete w1[i];
      w1[i]=NULL;
    }
    if(w2[i]!=NULL){
      delete w2[i];
      w2[i]=NULL;
    }
  }

  delete game1;
  game1=NULL;
  delete game2;
  game2=NULL;


  //MediaPlayer1->Close();
  //MediaPlayer2->Close();
  //MediaPlayer3->Close();
  //MediaPlayer4->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::crTimTimer(TObject *Sender)
{

  if(r2start){
    ro[p1]->crush(p1Pan);
    ro[p2]->crush(p2Pan);
  }else{
    ro[p1]->crush(r2Pan);
    ro[p2]->crush(r2Pan);
  }


  if(start1==false&&start2==false){
    pa=false;
    r2start=false;
    start1=true;
    start2=true;
    Sleep(2000);
    r2Pan->Left=200;
    r2Pan->Top=150;
    r2Pan->Width=450;
    r2Pan->Height=360;
    r2Pan->Visible=true;
    ro[p1]->Parent=r2Pan;
    ro[p2]->Parent=r2Pan;
    ro[p1]->Left=50; ro[p1]->Top=r2Pan->Height/2;
    ro[p2]->Left=r2Pan->Width-90;
    ro[p2]->Top=r2Pan->Height/2;
    ro[p1]->wlk->Enabled=true;
    ro[p2]->wlk->Enabled=true;
    p1Pan->Visible=false;
    p2Pan->Visible=false;
    goBut->Visible=false;
    Image8->Align=alClient;
    Image8->Visible=true;
    Image7->Align=alClient;
    Image7->Visible=true;
    Form1->Width=864;
    Form1->Height=635;
    Form1->Position=poScreenCenter;
    //MediaPlayer2->Stop();
    //MediaPlayer2->Rewind();
    //MediaPlayer3->Play();
    game1->time=50;
    game2->time=50;
    pa=true;
  }


  for(int i=0;i<p1n;i++){
    if(r2start&&(g1[i]->Top<=0||g1[i]->Top+g1[i]->Height>=p1Pan->Height||g1[i]->Left<=0||g1[i]->Left+g1[i]->Width>=p1Pan->Width)){
      g1[i]->die=false;
      for(int j=i;j<p1n-1;j++){
        g1[j]=g1[j+1];
      }
      p1n--;
    }
    if(r2start==false&&(g1[i]->Top<=0||g1[i]->Top+g1[i]->Height>=r2Pan->Height||g1[i]->Left<=0||g1[i]->Left+g1[i]->Width>=r2Pan->Width)){
      g1[i]->die=false;
      for(int j=i;j<p1n-1;j++){
        g1[j]=g1[j+1];
      }
      p1n--;
    }
    if(r2start==false&&(g1[i]->Top<ro[p2]->Top&&g1[i]->Top+20>=ro[p2]->Top&&g1[i]->Left<=ro[p2]->Left+35&&g1[i]->Left>=ro[p2]->Left-25)){
      ro[p2]->dehp(ro[p1]->mypow()+g1[i]->mypow());
      if(ro[p2]->hp<=0){
        over();
      }
      g1[i]->die=false;
      for(int j=i;j<p1n-1;j++){
        g1[j]=g1[j+1];
      }
      p1n--;
    }else{
      if(r2start==false&&(g1[i]->Top+30>ro[p2]->Top+50&&g1[i]->Top<=ro[p2]->Top+40&&g1[i]->Left<=ro[p2]->Left+35&&g1[i]->Left>=ro[p2]->Left-25)){
        ro[p2]->dehp(ro[p1]->mypow()+g1[i]->mypow());
        if(ro[p2]->hp<=0){
          over();
        }
        g1[i]->die=false;
        for(int j=i;j<p1n-1;j++){
          g1[j]=g1[j+1];
        }
        p1n--;
      }else{
        if(r2start==false&&(g1[i]->Left<ro[p2]->Left&&g1[i]->Left+20>=ro[p2]->Left&&g1[i]->Top<=ro[p2]->Top+45&&g1[i]->Top>=ro[p2]->Top-25)){
          ro[p2]->dehp(ro[p1]->mypow()+g1[i]->mypow());
          if(ro[p2]->hp<=0){
            over();
          }
          g1[i]->die=false;
          for(int j=i;j<p1n-1;j++){
            g1[j]=g1[j+1];
          }
          p1n--;
        }else{
          if(r2start==false&&(g1[i]->Left+30>ro[p2]->Left+40&&g1[i]->Left<=ro[p2]->Left+30&&g1[i]->Top<=ro[p2]->Top+45&&g1[i]->Top>=ro[p2]->Top-25)){
            ro[p2]->dehp(ro[p1]->mypow()+g1[i]->mypow());
            if(ro[p2]->hp<=0){
              over();
            }
            g1[i]->die=false;
            for(int j=i;j<p1n-1;j++){
              g1[j]=g1[j+1];
            }
            p1n--;
          }
        }
      }
    }
    if(r2start){
      for(int j=0;j<42;j++){
        if(w1[j]!=NULL){
          if(g1[i]->Top<w1[j]->Top&&g1[i]->Top+30>=w1[j]->Top&&g1[i]->Left<=w1[j]->Left+55&&g1[i]->Left>=w1[j]->Left-25){
            w1[j]->hp=w1[j]->hp-(g1[i]->mypow()+ro[p1]->mypow());
            if(w1[j]->hp<=0){
              fill(p1all1,p1all2,p1all3,p1all4,p1all5,1);
              delete w1[j];
              w1[j]=NULL;
            }
            g1[i]->die=false;
            for(int j=i;j<p1n-1;j++){
              g1[j]=g1[j+1];
            }
            p1n--;
          }else{
            if(g1[i]->Top+30>w1[j]->Top+50&&g1[i]->Top<=w1[j]->Top+50&&g1[i]->Left<=w1[j]->Left+55&&g1[i]->Left>=w1[j]->Left-25){
              w1[j]->hp=w1[j]->hp-(g1[i]->mypow()+ro[p1]->mypow());
              if(w1[j]->hp<=0){
                fill(p1all1,p1all2,p1all3,p1all4,p1all5,1);
                delete w1[j];
                w1[j]=NULL;
              }
              g1[i]->die=false;
              for(int j=i;j<p1n-1;j++){
                g1[j]=g1[j+1];
              }
              p1n--;
            }else{
              if(g1[i]->Left<w1[j]->Left&&g1[i]->Left+30>=w1[j]->Left&&g1[i]->Top<=w1[j]->Top+45&&g1[i]->Top>=w1[j]->Top-25){
                w1[j]->hp=w1[j]->hp-(g1[i]->mypow()+ro[p1]->mypow());
                if(w1[j]->hp<=0){
                  fill(p1all1,p1all2,p1all3,p1all4,p1all5,1);
                  delete w1[j];
                  w1[j]=NULL;
                }
                g1[i]->die=false;
                for(int j=i;j<p1n-1;j++){
                  g1[j]=g1[j+1];
                }
                p1n--;
              }else{
                if(g1[i]->Left+30>w1[j]->Left+60&&g1[i]->Left<=w1[j]->Left+60&&g1[i]->Top<=w1[j]->Top+45&&g1[i]->Top>=w1[j]->Top-25){
                  w1[j]->hp=w1[j]->hp-(g1[i]->mypow()+ro[p1]->mypow());
                  if(w1[j]->hp<=0){
                    fill(p1all1,p1all2,p1all3,p1all4,p1all5,1);
                    delete w1[j];
                    w1[j]=NULL;
                  }
                  g1[i]->die=false;
                  for(int j=i;j<p1n-1;j++){
                    g1[j]=g1[j+1];
                  }
                  p1n--;
                }
              }
            }
          }
        }
      }
    }
  }
  for(int i=0;i<p2n;i++){
    if(r2start&&(g2[i]->Top<=0||g2[i]->Top+g2[i]->Height>=p2Pan->Height||g2[i]->Left<=0||g2[i]->Left+g2[i]->Width>=p2Pan->Width)){
      g2[i]->die=false;
      for(int j=i;j<p2n-1;j++){
        g2[j]=g2[j+1];
      }
      p2n--;
    }
    if(r2start==false&&(g2[i]->Top<=0||g2[i]->Top+g2[i]->Height>=r2Pan->Height||g2[i]->Left<=0||g2[i]->Left+g2[i]->Width>=r2Pan->Width)){
      g2[i]->die=false;
      for(int j=i;j<p2n-1;j++){
        g2[j]=g2[j+1];
      }
      p2n--;
    }
    if(r2start==false&&(g2[i]->Top<ro[p1]->Top&&g2[i]->Top+20>=ro[p1]->Top&&g2[i]->Left<=ro[p1]->Left+35&&g2[i]->Left>=ro[p1]->Left-25)){
      ro[p1]->dehp(ro[p2]->mypow()+g2[i]->mypow());
      if(ro[p1]->hp<=0){
        over();
      }
      g2[i]->die=false;
      for(int j=i;j<p2n-1;j++){
        g2[j]=g2[j+1];
      }
      p2n--;
    }else{
      if(r2start==false&&(g2[i]->Top+30>ro[p1]->Top+50&&g2[i]->Top<=ro[p1]->Top+40&&g2[i]->Left<=ro[p1]->Left+35&&g2[i]->Left>=ro[p1]->Left-25)){
        ro[p1]->dehp(ro[p2]->mypow()+g2[i]->mypow());
        if(ro[p1]->hp<=0){
          over();
        }
        g2[i]->die=false;
        for(int j=i;j<p2n-1;j++){
          g2[j]=g2[j+1];
        }
        p2n--;
      }else{
        if(r2start==false&&(g2[i]->Left<ro[p1]->Left&&g2[i]->Left+20>=ro[p1]->Left&&g2[i]->Top<=ro[p1]->Top+45&&g2[i]->Top>=ro[p1]->Top-25)){
          ro[p1]->dehp(ro[p2]->mypow()+g2[i]->mypow());
          if(ro[p1]->hp<=0){
            over();
          }
          g2[i]->die=false;
          for(int j=i;j<p2n-1;j++){
            g2[j]=g2[j+1];
          }
          p2n--;
        }else{
          if(r2start==false&&(g2[i]->Left+30>ro[p1]->Left+40&&g2[i]->Left<=ro[p1]->Left+30&&g2[i]->Top<=ro[p1]->Top+45&&g2[i]->Top>=ro[p1]->Top-25)){
            ro[p1]->dehp(ro[p2]->mypow()+g2[i]->mypow());
            if(ro[p1]->hp<=0){
              over();
            }
            g2[i]->die=false;
            for(int j=i;j<p2n-1;j++){
              g2[j]=g2[j+1];
            }
            p2n--;
          }
        }
      }
    }
    if(r2start){
      for(int j=0;j<42;j++){
        if(w2[j]!=NULL){
          if(g2[i]->Top<w2[j]->Top&&g2[i]->Top+30>=w2[j]->Top&&g2[i]->Left<=w2[j]->Left+55&&g2[i]->Left>=w2[j]->Left-25){
            w2[j]->hp=w2[j]->hp-(g2[i]->mypow()+ro[p2]->mypow());
            if(w2[j]->hp<=0){
              fill(p2all1,p2all2,p2all3,p2all4,p2all5,2);
              delete w2[j];
              w2[j]=NULL;
            }
            g2[i]->die=false;
            for(int j=i;j<p2n-1;j++){
              g2[j]=g2[j+1];
            }
            p2n--;
          }else{
            if(g2[i]->Top+30>w2[j]->Top+50&&g2[i]->Top<=w2[j]->Top+50&&g2[i]->Left<=w2[j]->Left+55&&g2[i]->Left>=w2[j]->Left-25){
              w2[j]->hp=w2[j]->hp-(g2[i]->mypow()+ro[p2]->mypow());
              if(w2[j]->hp<=0){
                fill(p2all1,p2all2,p2all3,p2all4,p2all5,2);
                delete w2[j];
                w2[j]=NULL;
              }
              g2[i]->die=false;
              for(int j=i;j<p2n-1;j++){
                g2[j]=g2[j+1];
              }
              p2n--;
            }else{
              if(g2[i]->Left<w2[j]->Left&&g2[i]->Left+30>=w2[j]->Left&&g2[i]->Top<=w2[j]->Top+45&&g2[i]->Top>=w2[j]->Top-25){
                w2[j]->hp=w2[j]->hp-(g2[i]->mypow()+ro[p2]->mypow());
                if(w2[j]->hp<=0){
                  fill(p2all1,p2all2,p2all3,p2all4,p2all5,2);
                  delete w2[j];
                  w2[j]=NULL;
                }
                g2[i]->die=false;
                for(int j=i;j<p2n-1;j++){
                  g2[j]=g2[j+1];
                }
                p2n--;
              }else{
                if(g2[i]->Left+30>w2[j]->Left+60&&g2[i]->Left<=w2[j]->Left+60&&g2[i]->Top<=w2[j]->Top+45&&g2[i]->Top>=w2[j]->Top-25){
                  w2[j]->hp=w2[j]->hp-(g2[i]->mypow()+ro[p2]->mypow());
                  if(w2[j]->hp<=0){
                    fill(p2all1,p2all2,p2all3,p2all4,p2all5,2);
                    delete w2[j];
                    w2[j]=NULL;
                  }
                  g2[i]->die=false;
                  for(int j=i;j<p2n-1;j++){
                    g2[j]=g2[j+1];
                  }
                  p2n--;
                }
              }
            }
          }
        }
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::shootingTimer(TObject *Sender)
{
  if(GetAsyncKeyState(81)) p1t++;
  if(GetAsyncKeyState(69)) p1t--;
  if(p1t==5) p1t=0;
  if(p1t==-1) p1t=4;

  if(GetAsyncKeyState(79)) p2t++;
  if(GetAsyncKeyState(73)) p2t--;
  if(p2t==5) p2t=0;
  if(p2t==-1) p2t=4;

  TPanel *pan1=p1Pan,*pan2=p2Pan;
  if(r2start==false){
    pan1=r2Pan; pan2=r2Pan;
  }
  if((GetAsyncKeyState(32)&&start1||GetAsyncKeyState(32)&&r2start==false)&&pa){
    Shooting(0,p1,p1a1,p1all1,p1t,g1,p1n,0,5,3,pan1);
    Shooting(1,p1,p1a2,p1all2,p1t,g1,p1n,1,5,3,pan1);
    Shooting(2,p1,p1a3,p1all3,p1t,g1,p1n,2,5,4,pan1);
    Shooting(3,p1,p1a4,p1all4,p1t,g1,p1n,3,5,3,pan1);
    Shooting(4,p1,p1a5,p1all5,p1t,g1,p1n,4,5,4,pan1);
  }
  if((GetAsyncKeyState(80)&&start2||GetAsyncKeyState(80)&&r2start==false)&&pa){
    Shooting(0,p2,p2a1,p2all1,p2t,g2,p2n,0,5,3,pan2);
    Shooting(1,p2,p2a2,p2all2,p2t,g2,p2n,1,5,3,pan2);
    Shooting(2,p2,p2a3,p2all3,p2t,g2,p2n,2,5,4,pan2);
    Shooting(3,p2,p2a4,p2all4,p2t,g2,p2n,3,5,3,pan2);
    Shooting(4,p2,p2a5,p2all5,p2t,g2,p2n,4,5,4,pan2);
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::goButClick(TObject *Sender)
{
  //MediaPlayer1->Stop();
  //MediaPlayer1->Rewind();
  //MediaPlayer2->Play();

  goBut->Enabled=false;
  int L1=0,T1=2,L2=0,T2=2;
  for(int i=0;i<42;i++){
    if(random(2)==0){
      w1[i]=new wall();
      w1[i]->Parent=p1Pan;
      if(L1*60>=p1Pan->Width){L1=0;T1++;}
      w1[i]->Left=(L1++)*60;
      w1[i]->Top=50*T1;
    }else{
      L1++;
    }

    if(random(2)==0){
      w2[i]=new wall();
      w2[i]->Parent=p2Pan;
      if(L2*60>=p1Pan->Width){L2=0;T2++;}
      w2[i]->Left=(L2++)*60;
      w2[i]->Top=50*T2;
    }else{
      L2++;
    }
  }

  if(gamet){
    gamet=false;
    game1=new game(p1Pan,t1Lab,1);
    game2=new game(p2Pan,t2Lab,2);
  }else{
    game1->Enabled=true;
    game2->Enabled=true;
  }
  pa=true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TForm1::agButClick(TObject *Sender)
{
  for(int i=0;i<42;i++){
    if(w1[i]!=NULL){
      delete w1[i];
      w1[i]=NULL;
    }
    if(w2[i]!=NULL){
      delete w2[i];
      w2[i]=NULL;
    }
  }

  //MediaPlayer4->Stop();
  //MediaPlayer4->Rewind();
  //MediaPlayer1->Play();

  ro[p1]->Parent=p1Pan;
  ro[p2]->Parent=p2Pan;
  ro[p1]->Left=0; ro[p1]->Top=0;
  ro[p2]->Left=0; ro[p2]->Top=0;
  de1Lab->Caption=ro[p1]->deshow();
  de2Lab->Caption=ro[p2]->deshow();

  r2Pan->Left=200;
  r2Pan->Top=150;
  r2Pan->Width=450;
  r2Pan->Height=360;
  r2Pan->Visible=false;
  Image8->Visible=false;
  Image9->Visible=false;
  Image10->Visible=false;
  agBut->Visible=false;
  bBut->Visible=false;
  overLab->Visible=false;
  goBut->Enabled=true;

  p1all1=100; p1all2=0; p1all3=0; p1all4=0; p1all5=0;
  p2all1=100; p2all2=0; p2all3=0; p2all4=0; p2all5=0;
  p1a1=0; p2a1=0; p1a2=0; p2a2=0; p1a3=0; p2a3=0; p1a4=0; p2a4=0; p1a5=0; p2a5=0;
  d1Lab->Caption="";
  d2Lab->Caption="";
  t1Lab->Caption=50;
  t2Lab->Caption=50;

  Form1->Width=1100;
  Form1->Height=900;
  Form1->Position=poScreenCenter;
  Image2->Visible=true;
  Image2->Align=alClient;

  p1Pan->Visible=true;
  p2Pan->Visible=true;
  goBut->Visible=true;
  Image5->Visible=true;
  Image6->Visible=true;
  t1Lab->Visible=true;
  t2Lab->Visible=true;
  de1Lab->Visible=true;
  de2Lab->Visible=true;
  r2start=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::bButClick(TObject *Sender)
{
  //MediaPlayer4->Stop();
  //MediaPlayer4->Rewind();
  //MediaPlayer1->Play();

  for(int i=0;i<usernum;i++){
    delete ro[i];
    ro[i]=NULL;
  }
  for(int i=0;i<42;i++){
    if(w1[i]!=NULL){
      delete w1[i];
      w1[i]=NULL;
    }
    if(w2[i]!=NULL){
      delete w2[i];
      w2[i]=NULL;
    }
  }
  if(usernum!=0){
    ro[0]=new Role(1,10);
    ro[0]->load(ro);
  }

  p1Pan->Visible=false;
  p2Pan->Visible=false;
  goBut->Visible=false;
  Image5->Visible=false;
  Image6->Visible=false;
  t1Lab->Visible=false;
  t2Lab->Visible=false;
  de1Lab->Visible=false;
  de2Lab->Visible=false;



  Form1->Width=341;
  Form1->Height=585;
  Form1->Position=poScreenCenter;
  Image1->Visible=true;
  stBut->Visible=true;
  regisBut->Visible=true;
  helpBut->Visible=true;
  goBut->Enabled=true;
  Image8->Visible=false;
  r2Pan->Visible=false;
  Image9->Visible=false;
  Image10->Visible=false;
  agBut->Visible=false;
  bBut->Visible=false;
  overLab->Visible=false;
  p1all1=100; p1all2=0; p1all3=0; p1all4=0; p1all5=0;
  p2all1=100; p2all2=0; p2all3=0; p2all4=0; p2all5=0;
  p1a1=0; p2a1=0; p1a2=0; p2a2=0; p1a3=0; p2a3=0; p1a4=0; p2a4=0; p1a5=0; p2a5=0;
  d1Lab->Caption="";
  d2Lab->Caption="";
  t1Lab->Caption=50;
  t2Lab->Caption=50;
  r2start=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::helpButClick(TObject *Sender)
{
  ShowMessage("1P操作: 上-w下-s左-a右-d   換彈-q e  發射-空白鍵\n2P操作: 上下左右-方向鍵    換彈-I o   發射-p\n\n遊戲規則:\n遊戲分為兩階段，玩家一開始擁有火焰100發，第一階段目的為收集能量彈，在\n時間內打破魔鏡能收集5種隨機能量彈、幫自己加秒數、幫敵人扣秒數，兩方時\n間歸零後傳送到第二階段場地，已收集的子彈互相對戰，寫條歸零獲勝，勝者獲\n得經驗25點，滿100點升一等並獲得力量+10、血量+100。\n\n補充\n能量彈基礎威力=>火100、暗200、幻250、水300、光400\n玩家每一擊威力為基礎威力+角色力量");
}
//---------------------------------------------------------------------------



