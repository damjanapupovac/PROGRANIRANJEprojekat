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

nalog* create(char ime[], char prezime[], char email[], char username[], char password[] ,nalog* next) // 30, NULL
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

nalog* append(nalog* head, char ime[], char prezime[], char email[], char username[], char password[] ,nalog* next)
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
    nalog* cursor=NULL;
    nalog* prethodni=NULL;
    cursor=head;
    //prethodni=NULL;
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

nalog* izmena_naloga(nalog* head, char* username)
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

int main()
{
    int naredba;
    nalog* nalozi=NULL;
    nalog* ulogovani=NULL;
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
                nalozi=izmena_naloga(nalozi, ulogovani->username);
                break;
            case 5:
                nalozi=dispose(nalozi, ulogovani->username);
                ulogovani=NULL;
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
