#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nalog
{
    char ime[20];
    char prezime[20];
    char email[20];
    char username[20];
    char password[20];
    struct nalog* next;
}nalog;

typedef struct video
{
    char naziv[100];
    char link[6];
    char usernamev[16];
    int brpregleda;
    int brlajkova;
    int brdislajkova;
    int brkomentara;
    struct video* sledeci;
}video;

nalog* create(char ime[], char prezime[], char email[], char username[], char password[] ,nalog* next)
{
    nalog* novi_nalog = (nalog*)malloc(sizeof(nalog));
    strcpy(novi_nalog->ime, ime);
    strcpy(novi_nalog->prezime, prezime);
    strcpy(novi_nalog->email, email);
    strcpy(novi_nalog->username, username);
    strcpy(novi_nalog->password, password);
    novi_nalog->next = next;

    return novi_nalog;
}

nalog* append(nalog* head, char ime[], char prezime[], char email[], char username[], char password[], nalog* next)
{
    nalog* novi_nalog;

    if(head == NULL)
    {
        novi_nalog = create(ime, prezime, email, username, password, head);
        head = novi_nalog;
        return head;
    }
    nalog *cursor = head;

    while(cursor->next != NULL)
        cursor = cursor->next;
    novi_nalog = create(ime, prezime, email, username, password, NULL);
    cursor->next = novi_nalog;
    return head;
}

int proveraemaila(nalog* head, char* email)
{
    nalog* cursor=head;
    while(cursor!=NULL)
    {
        if(strcmp(cursor->email, email)==0)
        {
            return 1;
        }
        cursor=cursor->next;
    }
    return 0;
}

int proverausernamea(nalog* head, char* username)
{
    nalog* cursor=head;
    while(cursor!=NULL)
    {
        if(strcmp(cursor->username, username)==0)
        {
            return 1;
        }
        cursor=cursor->next;
    }
    return 0;
}

int EmailValidan(char* email)
{
    int i;
    int pozicija=0;
    int pozicija2=0;
    for(i=0; i<29;i++)
    {
        if(email[i]=='@')
        {
            pozicija=i;
            break;
        }
    }
    if(pozicija==0)
    {
        return 1;
    }
    for(i=pozicija+1; i<29; i++)
    {
        if(email[i]=='.')
        {
            pozicija2=i;
            break;
        }
    }
    if(pozicija2==0)
    {
        return 1;
    }
    return 0;
}

void dodajufajl(char* ime, char* prezime, char* email, char* username, char* password)
{
    FILE* fajlsanalozima= fopen("nalozi.txt", "a+");
    fprintf(fajlsanalozima, "%s %s %s %s %s\n", ime, prezime, email, username, password);
    fclose(fajlsanalozima);
}

nalog* citanjeizfajla(nalog* nalozi)
{
    char ime[20]="";
    char prezime[20]="";
    char email[30]="";
    char username[16]="";
    char password[16]="";
    FILE* fajlsanalozima= fopen("nalozi.txt", "r");
    while(1==1)
    {
        int rezultatcitanja=fscanf(fajlsanalozima, "%s %s %s %s %s", ime, prezime, email, username, password);
        if(rezultatcitanja==EOF)
        {
            break;
        }
        nalozi = append(nalozi, ime, prezime, email, username, password, NULL);
    }
    fclose(fajlsanalozima);
    return nalozi;
}

nalog* DodavanjeNovogNaloga(nalog* nalozi)
{
    char ime[20];
    char prezime[20];
    char email[30];
    char username[16];
    char password[16];
    char password2[16];
    int provera1;
    int provera2;
    int emailvalidan;
    printf("Izabrali ste dodavanje novog naloga.\n");
    printf("Unesite vase ime: ");
    scanf("%s", ime);
    printf("\nUnesite vase prezime: ");
    scanf("%s", prezime);
    printf("\nUnesite vas email: ");
    scanf("%s", email);
    emailvalidan=EmailValidan(email);
    if(emailvalidan==1)
    {
        printf("Email nije validan. Pokusajte ponovo.");
        return nalozi;
    }
    provera1=proveraemaila(nalozi, email);
    if(provera1==1)
    {
        printf("Email je vec iskoristen. Pokusajte ponovo.");
        return nalozi;
    }
    printf("\nUnesite vas username: ");
    scanf("%s", username);
    provera2=proverausernamea(nalozi, username);
    if(provera2==1)
    {
        printf("Username je vec iskoristen. Pokusajte ponovo.");
        return nalozi;
    }
    printf("\nUnesite vas password: ");
    scanf("%s", password);
    printf("\nUnesite vas password ponovo: ");
    scanf("%s", password2);
    if(strcmp(password, password2)!=0)
    {
        printf("Passwordi se ne poklapaju. Pokusajte ponovo.");
        return nalozi;
    }
        nalozi = append(nalozi, ime, prezime, email, username, password, NULL);
        dodajufajl(ime, prezime, email, username, password);
    return nalozi;
}

nalog* nadji_nalog(nalog* nalozi, char* UsernameIliEmail, char* password)
{
    nalog* cursor=nalozi;
    while(cursor!=NULL)
    {
        if(strcmp(UsernameIliEmail, cursor->email)==0 || strcmp(UsernameIliEmail, cursor->username)==0)
        {
            if(strcmp(password, cursor->password)==0)
            {
                return cursor;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            cursor=cursor->next;
        }
    }
}

nalog* Login(nalog* nalozi)
{
    char UsernameIliEmail[30];
    char password[16];
    printf("Unesite username ili email: ");
    scanf("%s", UsernameIliEmail);
    printf("\nUnesite password: ");
    scanf("%s", password);
    nalog* ulogovan=1;
    ulogovan=nadji_nalog(nalozi, UsernameIliEmail, password);
    //printf("%s", ulogovan->username);
    return ulogovan;
}

nalog* dispose(nalog* head, char* username)
{
    char ime;
    char prezime;
    char email;
    char password;
    nalog* cursor=NULL;
    nalog* prethodni=NULL;
    cursor=head;
    if(strcmp(head->username, username)==0)
    {
        head=cursor->next;
        free(cursor);
    }
    else
    {
        prethodni=cursor;
        cursor=cursor->next;
        while(cursor!=NULL)
        {
            if(strcmp(cursor->username, username)==0)
            {
                prethodni->next=cursor->next;
                free(cursor);
                if(prethodni->next==NULL)
                {
                    cursor=NULL;
                }
            }
            else
            {
                prethodni=cursor;
                cursor=cursor->next;
            }
        }
    }
    return head;
}

nalog* izmena_naloga(nalog* head, char* username, char* email)
{
    char novo_ime[20];
    char novo_prezime[20];
    char novi_password[16];
    printf("Unesite novo ime: ");
    scanf("%s", novo_ime);
    printf("Unesite novo prezime: ");
    scanf("%s", novo_prezime);
    printf("Unesite novi password: ");
    scanf("%s", novi_password);
    nalog* cursor=head;
    while(cursor!=NULL)
    {
        if(strcmp(cursor->username, username)==0)
        {
            strcpy(cursor->ime, novo_ime);
            strcpy(cursor->prezime, novo_prezime);
            strcpy(cursor->password, novi_password);
            return head;
        }
        cursor=cursor->next;
    }
    return head;
}

video* createv(char naziv[], char link[], int brpregleda, int brlajkova, int brdislajkova, int brkomentara, char* usernamev, video* sledeci)
{
    video* novi_video = (video*)malloc(sizeof(video));
    strcpy(novi_video->naziv, naziv);
    strcpy(novi_video->link, link);
    novi_video->brpregleda=brpregleda;
    novi_video->brlajkova=brlajkova;
    novi_video->brdislajkova=brdislajkova;
    novi_video->brkomentara=brkomentara;
    strcpy(novi_video->usernamev, usernamev);
    novi_video->sledeci = sledeci;
    return novi_video;
}

video* appendv(video* headv, char* naziv, char link[], int brpregleda, int brlajkova, int brdislajkova, int brkomentara, char* usernamev, video* sledeci)
{
    video* novi_video;

    if(headv == NULL)
    {
        novi_video = createv(naziv, link, brpregleda, brlajkova, brdislajkova, brkomentara, usernamev, headv);
        headv = novi_video;
        return headv;
    }
    video *cursorv = headv;

    while(cursorv->sledeci != NULL)
    {
        cursorv = cursorv->sledeci;
    }
    novi_video = createv(naziv, link, brpregleda, brlajkova, brdislajkova, brkomentara, usernamev, NULL);
    cursorv->sledeci = novi_video;
    return headv;
}

const char* random_link(char* s, const int duzina)
 {
    static const char niz[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    srand(time(0));
    for (int i = 0; i < duzina; ++i)
    {
        s[i] = niz[rand() % (sizeof(niz) - 1)];
    }

    s[duzina] = 0;
    return s;
}

void dodajufajlv(char* nazivVidea, char* link, int brpregleda, int brlajkova, int brdislajkova, int brkomentara, char*usernamev)
{
    FILE* fajlsavideima= fopen("videi.txt", "a+");
    fprintf(fajlsavideima, "%s %s %d %d %d %d %s\n", nazivVidea, link, brpregleda, brlajkova, brdislajkova, brkomentara, usernamev);
    fclose(fajlsavideima);
}

video* citanjeizfajlav(video* videi)
{
    char naziv[100]="";
    char link[6]="";
    int brpregleda=0;
    int brlajkova=0;
    int brdislajkova=0;
    int brkomentara=0;
    char usernamev[16];
    FILE* fajlsavideima= fopen("videi.txt", "r");
    while(1==1)
    {
        int rezultatcitanja=fscanf(fajlsavideima, "%s %s %d %d %d %d %s", naziv, link, &brpregleda, &brlajkova, &brdislajkova, &brkomentara, usernamev);
        if(rezultatcitanja==EOF)
        {
            break;
        }
        videi = appendv(videi, naziv, link, brpregleda, brlajkova, brdislajkova, brkomentara, usernamev, NULL);
    }
    fclose(fajlsavideima);
    return videi;
}

video* DodavanjeVidea(video* videi, char* username)
{
    char naziv[100]="";
    char link[6]="";
    int brpregleda=0;
    int brlajkova=0;
    int brdislajkova=0;
    int brkomentara=0;
    char usernamev[16];
    strcpy(usernamev, username);
    printf("Unesite naziv videa:\n");
    scanf("%s", naziv);
    const int duzina=5;
    char* link1=random_link(link, duzina);
    videi=appendv(videi, naziv, link1, brpregleda, brlajkova, brdislajkova, brkomentara, usernamev, NULL);
    dodajufajlv(naziv, link1, brpregleda, brlajkova, brdislajkova, brkomentara, usernamev);
    return videi;
}

video* izmeni_video(video* headv, char* naziv)
{
    char novi_naziv[100];
    video* cursorv=headv;
    printf("Unesite novi naziv: ");
    scanf("%s", novi_naziv);
    strcpy(cursorv->naziv, novi_naziv);
    return headv;
}

video* disposev(video* headv, char* username, char* naziv)
{
    video* cursorv=NULL;
    video* prethodniv=NULL;
    int obrisan=0;
    cursorv=headv;
    if((strcmp(headv->naziv, naziv)==0) && (strcmp(headv->usernamev, username)==0))
    {
        headv=cursorv->sledeci;
        free(cursorv);
        obrisan=1;
    }
    else
    {
        prethodniv=cursorv;
        cursorv=cursorv->sledeci;
        while(cursorv!=NULL)
        {
            if((strcmp(cursorv->naziv, naziv)==0) && (strcmp(cursorv->usernamev, username)==0))
            {
                prethodniv->sledeci=cursorv->sledeci;
                free(cursorv);
                if(prethodniv->sledeci==NULL)
                {
                    cursorv=NULL;
                }
                obrisan=1;
            }
            else
            {
                prethodniv=cursorv;
                cursorv=cursorv->sledeci;
            }
        }
    }
    if(obrisan==0)
    {
        printf("Takav video ne postoji.");
    }
    return headv;
}

void sacuvajnaloge(nalog* nalozi)
{
    nalog* cursor=nalozi;
    FILE* fajlsanalozima=fopen("nalozi.txt", "w");
    while(cursor!=NULL)
    {
        fprintf(fajlsanalozima, "%s %s %s %s %s\n", cursor->ime, cursor->prezime, cursor->email, cursor->username, cursor->password);
        cursor=cursor->next;
    }
    fclose(fajlsanalozima);
}

void sacuvajvidee(video* videi)
{
    video* cursorv=videi;
    FILE* fajlsavideima=fopen("videi.txt", "w");
    while(cursorv!=NULL)
    {
        fprintf(fajlsavideima, "%s %s %d %d %d %d %s\n", cursorv->naziv, cursorv->link, cursorv->brpregleda, cursorv->brlajkova, cursorv->brdislajkova, cursorv->brkomentara, cursorv->usernamev);
        cursorv=cursorv->sledeci;
    }
    fclose(fajlsavideima);
}

video* appendpretrage(video* headv, video* vid)
{
    video* novi_video;
    novi_video = createv(vid->naziv, vid->link, vid->brpregleda, vid->brlajkova, vid->brdislajkova, vid->brkomentara, vid->usernamev, NULL);
    video* prethodni=NULL;
    if(headv == NULL)
    {
        headv = novi_video;
        novi_video->sledeci=NULL;
        return headv;
    }
    if(novi_video->brpregleda>headv->brpregleda)
    {
        novi_video->sledeci=headv;
        headv=novi_video;
        return headv;
    }
    prethodni=headv;
    video* cursorv = prethodni->sledeci;
    while(cursorv != NULL)
    {
        if(cursorv->brpregleda<novi_video->brpregleda)
        {
            prethodni->sledeci=novi_video;
            novi_video->sledeci=cursorv;
            return headv;
        }
        prethodni=cursorv;
        cursorv = cursorv->sledeci;
    }
    //video = createv(naziv, link, brpregleda, brlajkova, brdislajkova, brkomentara, usernamev, NULL);
    prethodni->sledeci=novi_video;
    novi_video->sledeci=NULL;
    return headv;
}

void pretraga(video* headv, char* naziv)
{
    video* rezultatipretrage=NULL;
    video* cursorv=headv;
    while(cursorv!=NULL)
    {
        if(strcmp(naziv, cursorv->naziv)==0)
        {
            rezultatipretrage=appendpretrage(rezultatipretrage, cursorv);
        }
        cursorv=cursorv->sledeci;
    }

    IspisiVidee(rezultatipretrage, 0);
}

void IspisiVidee(video* videi, int i)
{
    video* cursorv=videi;
    int brojac=0;
    while(cursorv!=NULL && (brojac<i || i==0))
    {
        printf("%s %s %d %d %d %d %s\n", cursorv->naziv, cursorv->link, cursorv->brpregleda, cursorv->brlajkova, cursorv->brdislajkova, cursorv->brkomentara, cursorv->usernamev);
        cursorv=cursorv->sledeci;
        brojac++;
    }
}

video* GledajVideo(video* headv, char* link)
{
    video* cursorv=headv;
     while(cursorv!=NULL)
    {
        if(strcmp(link, cursorv->link)==0)
        {
            cursorv->brpregleda=cursorv->brpregleda+1;
            return headv;
        }
        cursorv=cursorv->sledeci;
    }
    return headv;
}

video* like_dislike_comment (video* headv, int naredba)
{
    video* cursorv=headv;
     switch(naredba)
    {
    case 1:
        cursorv->brlajkova=cursorv->brlajkova+1;
        break;
    case 2:
        cursorv->brdislajkova=cursorv->brdislajkova+1;
        break;
    case 3:
        cursorv->brkomentara=cursorv->brkomentara+1;
        break;
    case 0:
        break;
    default:
        printf("Pogrešna naredba. Pokušajte ponovo.\n");
        break;
    }
    return headv;
}

void HomePage (video* videi)
{
    video* nova_lista=NULL;
    video* cursorv=videi;
    while(cursorv!=NULL)
    {
        nova_lista=appendpretrage(nova_lista, cursorv);
        cursorv=cursorv->sledeci;
    }
    IspisiVidee(nova_lista, 10);
}

int main()
{
    int naredba;
    nalog* nalozi=NULL;
    video* videi=NULL;
    nalog* ulogovani=NULL;

    videi=citanjeizfajlav(videi);
    nalozi=citanjeizfajla(nalozi);

    FILE* fajlsavideima;
    FILE* fajlsanalozima;

    char naziv_videa[100];
    char link[6];
    int naredba1;
    do
     {
        printf("\nMeni\n");
        printf("1. Dodavanje novog naloga\n");
        printf("2. Log in\n");
        if(ulogovani!=NULL)
        {
           printf("3. Log out\n");
           printf("4. Izmena naloga\n");
           printf("5. Brisanje naloga\n");
           printf("6. Dodavanje videa\n");
           printf("7. Izmena videa\n");
           printf("8. Brisanje videa\n");
           printf("9. HomePage\n");
           printf("10. Pretraga\n");
        }
        printf("0. Kraj\n");
        scanf("%d",&naredba);
        switch (naredba)
         {
            case 1:
                nalozi = DodavanjeNovogNaloga(nalozi);
                break;
            case 2:
                ulogovani=Login(nalozi);
                break;
            case 3:
                ulogovani=NULL;
                break;
            case 4:
                nalozi=izmena_naloga(nalozi, ulogovani->username, ulogovani->email);
                sacuvajnaloge(nalozi);
                break;
            case 5:
                nalozi=dispose(nalozi, ulogovani->username);
                sacuvajnaloge(nalozi);
                ulogovani=NULL;
                break;
            case 6:
                videi=DodavanjeVidea(videi, ulogovani->username);
                break;
            case 7:
                videi=izmeni_video(videi, videi->naziv);
                sacuvajvidee(videi);
                break;
            case 8:
                printf("Unesite naziv videa koji zelite da obrisete:");
                scanf("%s", naziv_videa);
                videi=disposev(videi, videi->usernamev, naziv_videa);
                sacuvajvidee(videi);
                break;
            case 9:
                HomePage(videi);
                printf("Unesite link videa koji hocete da gledate: ");
                scanf("%s", link);
                videi=GledajVideo(videi, link);
                printf("Like(1)   Dislike(2)   Comment(3)   0\n");
                scanf("%d", &naredba1);
                videi=like_dislike_comment(videi, naredba1);
                sacuvajvidee(videi);
                break;
            case 10:
                printf("Unesite ime videa: ");
                scanf("%s", naziv_videa);
                pretraga(videi, naziv_videa);
                printf("Unesite link videa koji hocete da gledate: ");
                scanf("%s", link);
                videi=GledajVideo(videi, link);
                printf("Like(1)   Dislike(2)   Comment(3)   0\n");
                scanf("%d", &naredba1);
                videi=like_dislike_comment(videi, naredba1);
                sacuvajvidee(videi);
                break;
            case 0:
                break;
            default:
                printf("Pogrešna naredba. Pokušajte ponovo.\n");
                break;
         }
     } while (naredba != 0);
    return 0;
}
