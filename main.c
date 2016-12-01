#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<ctype.h>
char empty_board[9][9]= {{' ','A','B','C','D','E','F','G','H'},
    {'8','-','.','-','.','-','.','-','.'},{'7','.','-','.','-','.','-','.','-'},
    {'6','-','.','-','.','-','.','-','.'},{'5','.','-','.','-','.','-','.','-'},
    {'4','-','.','-','.','-','.','-','.'},{'3','.','-','.','-','.','-','.','-'},
    {'2','-','.','-','.','-','.','-','.'},{'1','.','-','.','-','.','-','.','-'}//with no pieces
};
char new_board[9][9]= {{' ','A','B','C','D','E','F','G','H'},{'8','R','N','B','Q','K','B','N','R'},
    {'7','P','P','P','P','P','P','P','P'},{'6','-','.','-','.','-','.','-','.'},{'5','.','-','.','-','.','-','.','-'},
    {'4','-','.','-','.','-','.','-','.'},{'3','.','-','.','-','.','-','.','-'},
    {'2','p','p','p','p','p','p','p','p'},{'1','r','n','b','q','k','b','n','r'}//displayed in the beginning of the game
};
int check_after=0;
int checkmate=0,threatc=0;
char used_board[9][9];//used in the game
char taken_white [16];
int taken_whitec=0;
char taken_black [16];
int taken_blackc=0;
char white_team_used[5][16];
char black_team_used[5][16];
char white_team [5][16]=
{
    {'r','n','b','q','k','b','n','r','p','p','p','p','p','p','p','p'},
    {'8','8','8','8','8','8','8','8','7','7','7','7','7','7','7','7'},    //row index
    {'A','B','C','D','E','F','G','H','A','B','C','D','E','F','G','H'},    //col index
    {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},    //in the game(1)or out(0)
    {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};    //moved before(0)or not(1) for the king,rook and pawn

char black_team [5][16]=
{
    {'R','N','B','Q','K','B','N','R','P','P','P','P','P','P','P','P'},
    {'1','1','1','1','1','1','1','1','2','2','2','2','2','2','2','2'},    //row index
    {'A','B','C','D','E','F','G','H','A','B','C','D','E','F','G','H'},    //col index
    {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},    //in the game(1)or out(0)
    {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};    //moved before(0)or not(1) for the king,rook and pawn
int undo[7][200]= {0}; //row1: rowindex1 row2: colindex1 row3:rowindex2 row4:colindex2 promotion takenout CHECK
int undoc = 0,moved=0,playerc=0;
int redo [6][200]= {0} , redoc = 0;
int redo_flag=0;
char propiece;
int generation [9][9]={0};
int moves_hint [2][14];
int moves_counter = 0;
int possible = 1 ;
int threat =0;
void possible_moves(char myteam [5][16], char otherteam[5][16])//to find if there is a possible move for a team or not
{
    int i , x , r , c;
    for(i=0 ; i<16 ; i++)
    {
        if(myteam[i][3]=='0')
        {
            continue;
        }
        r=(int)myteam[1][i]-48;
        c=(int)myteam[2][i]-64;
        char piece = myteam[0][i];
        switch(piece)
        {
        case 'r':
        case 'R':
            x=generate_r(r,c,piece,myteam,otherteam);
            break;
        case 'N':
        case 'n':
            x=generate_n(r,c,piece,myteam,otherteam);
            break;
        case 'b':
        case 'B':
            x=generate_b(r,c,piece,myteam,otherteam);
            break;
        case 'q':
        case 'Q':
            x=generate_b(r,c,piece,myteam,otherteam);
            x=generate_r(r,c,piece,myteam,otherteam);
            break;
        case 'k':
        case 'K':
            x=generate_k(r,c,piece,myteam,otherteam);
            break;
        case 'p':
            x=generate_p(r,c,piece,myteam,otherteam);
            break;
        case 'P':
            x=generate_P(r,c,piece,myteam,otherteam);
            break;
        default:
            printf("ERROR");

        }
        if (x==1)
        {
            possible = 1;
            break ;
        }
    }
}

void hint (int r , int c) //find the valid squares for the selected piece
{
    int i;
    char mp = used_board[r][c];
    switch (mp)
    {
    case 'b':
        generate_b(r,c,mp,white_team_used,black_team_used);
        break;
    case'B':
        generate_b(r,c,mp,black_team_used,white_team_used);
        break;
    case'p':
        generate_p(r,c,mp,white_team_used,black_team_used);
        break;
    case'P':
        generate_P(r,c,mp,black_team_used,white_team_used);
        break;
    case'R':
        generate_r(r,c,mp,black_team_used,white_team_used);
        break;
    case'r':
        generate_r(r,c,mp,white_team_used,black_team_used);
        break;
    case'N':
        generate_n(r,c,mp,black_team_used,white_team_used);
        break;
    case'n':
        generate_n(r,c,mp,white_team_used,black_team_used);
        break;
    case'q':
        generate_r(r,c,mp,white_team_used,black_team_used);
        generate_b(r,c,mp,white_team_used,black_team_used);
        break;
    case'Q':
        generate_r(r,c,mp,black_team_used,white_team_used);
        generate_b(r,c,mp,black_team_used,white_team_used);
        break;
    case'k':
        generate_k(r,c,mp,white_team_used,black_team_used);
        break;
    case'K':
        generate_k(r,c,mp,black_team_used,white_team_used);
        break;
    default:
        printf("error int the hint switch");
        break;
    }
    printf("\n");
    if (moves_counter==0)
    {
        printf("no possible moves\n");
    }
    else
    {
        for (i=0 ; i<moves_counter ; i++)
        {
            printf(" %c%c  ",moves_hint[1][i]+64,9+48-moves_hint[0][i]);
        }
    }
    printf("\n");
}

void redo_function()
{
    moved=1;
    int i;
    redoc--;
    undo[0][undoc]=redo[0][redoc]; //save the move to undo it later
    undo[1][undoc]=redo[1][redoc];
    undo[2][undoc]=redo[2][redoc];
    undo[3][undoc]=redo[3][redoc];
    undo[4][undoc]=redo[4][redoc];
    undo[5][undoc]=redo[5][redoc];
    undoc++;
    int pr = redo[0][redoc];
    int pc = redo[1][redoc];
    int dr = redo[2][redoc];
    int dc = redo[3][redoc];
    used_board[dr][dc]=used_board[pr][pc];
    if (playerc%2==0)//redo player 1 game
    {
        for (i=0 ; i<16 ; i++)
        {
            if ((int)white_team_used[1][i]-48==pr && (int)white_team_used[2][i]-64==pc)
            {
                white_team_used[1][i]=(char)dr+48 ;
                white_team_used[2][i]=(char)dc+64 ;
                break;
            }
        }
        if (redo[5][redoc]==1)   //1 for taken black
        {
            taken_blackc++;
            taken_black[taken_blackc]=used_board[pr][pc];
            for (i=0 ; i<16 ; i++)
            {
                if ((int)black_team_used[1][i]-48==pr && (int)black_team_used[2][i]-64==pc)/////////////
                {
                    black_team_used[3][i]='0';
                    break;
                }
            }
            used_board[pr][pc]=empty_board[pr][pc];
        }
        else
            used_board[pr][pc]=empty_board[pr][pc];
    }
    else
    {
        for (i=0 ; i<16 ; i++)
        {
            if ((int)black_team_used[1][i]-48==pr && (int)black_team_used[2][i]-64==pc)////////
            {
                black_team_used[1][i]=(char)dr+48 ;
                black_team_used[2][i]=(char)dc+64 ;
                break;
            }
        }
        if (redo[5][redoc]==2)   //2 for taken white
        {
            taken_whitec++;
            taken_white[taken_whitec]=used_board[pr][pc];
            for (i=0 ; i<16 ; i++)
            {
                if ((int)white_team_used[1][i]-48==pr && (int)white_team_used[2][i]-64==pc)/////////
                {
                    white_team_used[3][i]='0';
                    break;
                }
            }
            used_board[pr][pc]=empty_board[pr][pc];
        }
        else
            used_board[pr][pc]=empty_board[pr][pc];

    }
    if (redo[4][redoc]!=0)
    {
        used_board[dr][dc]=(char)redo[4][redoc];
    }
}

void undo_function ()
{
    moved=1;
    int i;
    undoc--;
    if(redo_flag==1)
    {
        redo[0][redoc]=undo[0][undoc];//save in redo to do it later
        redo[1][redoc]=undo[1][undoc];
        redo[2][redoc]=undo[2][undoc];
        redo[3][redoc]=undo[3][undoc];
        redo[4][redoc]=undo[4][undoc];
        redo[5][redoc]=undo[5][undoc];
        redoc++;
        redo_flag=0;

    }
    int dr = undo[0][undoc];
    int dc = undo[1][undoc];
    int pr = undo[2][undoc];
    int pc = undo[3][undoc];
    used_board[dr][dc]=used_board[pr][pc];
    if (playerc%2==1)//undo player 1 game
    {
        for (i=0 ; i<16 ; i++)
        {
            if ((int)white_team_used[1][i]-48==pr && (int)white_team_used[2][i]-64==pc)//////////////
            {
                white_team_used[1][i]=(char)dr+48 ;
                white_team_used[2][i]=(char)dc+64 ;
                break;
            }
        }
        if (undo[5][undoc]==1)   //1 for taken black
        {
            taken_blackc--;
            used_board[pr][pc]=taken_black[taken_blackc];
            for (i=0 ; i<16 ; i++)
            {
                if ((int)black_team_used[1][i]-48==pr && (int)black_team_used[2][i]-64==pc)/////////////
                {
                    black_team_used[3][i]='1';
                    break;
                }
            }
        }
        else
            used_board[pr][pc]=empty_board[pr][pc];
        if (undo[4][undoc]!=0)
        {
            used_board[dr][dc]='p';
        }
    }
    else
    {
        for (i=0 ; i<16 ; i++)
        {
            if ((int)black_team_used[1][i]-48==pr && (int)black_team_used[2][i]-64==pc)////////
            {
                black_team_used[1][i]=(char)dr+48 ;
                black_team_used[2][i]=(char)dc+64 ;
                break;
            }
        }
        if (undo[5][undoc]==2)   //2 for taken white
        {
            taken_whitec--;
            used_board[pr][pc]=taken_white[taken_whitec];
            for (i=0 ; i<16 ; i++)
            {
                if ((int)white_team_used[1][i]-48==pr && (int)white_team_used[2][i]-64==pc)
                {
                    white_team_used[3][i]='1';
                    break;
                }
            }
        }
        else
            used_board[pr][pc]=empty_board[pr][pc];
        if (undo[4][undoc]!=0)
        {
            used_board[dr][dc]='P';
        }
    }

}
void generate_p(int r , int c , char piece , char myteam[5][16] , char otherteam [5][16]) //generate all possible moves for p
{
    moves_counter=0;
    char dp;
    char mp = used_board[r][c];
    if(r-1>0&&c+1<9)
    {
        if(isupper((int)used_board[r-1][c+1]))
        {
            moves_hint[0][moves_counter]=r-1;
            moves_hint[1][moves_counter]=c+1;
            moves_counter++;
            dp = used_board[r-1][c+1];
            move(c, r, c+1, r-1, mp,dp);
            check_after=0;
            check(otherteam ,myteam);
            if (check_after==1)
            {
                moves_counter--;
            }
            undo_function();
            moved=0;
        }
    }
    if(r-1>0&&c-1>0)
    {
        if(isupper((int)used_board[r-1][c-1]))
        {
            moves_hint[0][moves_counter]=r-1;
            moves_hint[1][moves_counter]=c-1;
            moves_counter++;
            dp = used_board[r-1][c-1];
            move(c, r, c-1, r-1, mp,dp);
            check_after=0;
            check(otherteam ,myteam);
            if (check_after==1)
            {
                moves_counter--;
            }
            undo_function();
            moved=0;
        }
    }
    if(r-1>0)
    {
        if(used_board[r-1][c]==empty_board[r-1][c])
        {
            moves_hint[0][moves_counter]=r-1;
            moves_hint[1][moves_counter]=c;
            moves_counter++;
            dp = used_board[r-1][c];
            move(c, r, c, r-1, mp,dp);
            check_after=0;
            check(otherteam ,myteam);
            if (check_after==1)
            {
                moves_counter--;
            }
            undo_function();
            moved=0;
        }
    }
    if(r==7&&used_board[r-2][c]==empty_board[r-2][c]&&used_board[r-1][c]==empty_board[r-1][c])//move two squares forward
    {
        moves_hint[0][moves_counter]=r-2;
        moves_hint[1][moves_counter]=c;
        moves_counter++;
        dp = used_board[r-2][c];
        move(c, r, c, r-2, mp,dp);
        check_after=0;
        check(otherteam ,myteam);
        if (check_after==1)
        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }

}

void generate_P(int r , int c , char piece , char myteam[5][16] , char otherteam [5][16])
{
    moves_counter=0;
    char dp;
    char mp = used_board[r][c];
    if(r+1<9&&c+1<9)
    {

        if(islower((int)used_board[r+1][c+1]))
        {
            moves_hint[0][moves_counter]=r+1;
            moves_hint[1][moves_counter]=c+1;
            moves_counter++;
            dp = used_board[r+1][c+1];
            move(c, r, c+1, r+1, mp,dp);
            check_after=0;
            check(otherteam ,myteam);
            if (check_after==1) //if it causes threat on my king
            {
                moves_counter--;
            }
            undo_function();
            moved=0;
        }
    }
    if(r+1<9&&c-1>0)
    {
        if(islower((int)used_board[r+1][c-1]))
        {
            moves_hint[0][moves_counter]=r+1;
            moves_hint[1][moves_counter]=c-1;
            moves_counter++;
            dp = used_board[r+1][c-1];
            move(c, r, c-1, r+1, mp,dp);
            check_after=0;
            check(otherteam ,myteam);
            if (check_after==1)
            {
                moves_counter--;
            }
            undo_function();
            moved=0;
        }
    }
    if(r+1<9)
    {
        if(used_board[r+1][c]==empty_board[r+1][c])
        {
            moves_hint[0][moves_counter]=r+1;
            moves_hint[1][moves_counter]=c;
            moves_counter++;
            dp = used_board[r+1][c];
            move(c, r, c, r+1, mp,dp);
            check_after=0;
            check(otherteam ,myteam);
            if (check_after==1)
            {
                moves_counter--;
            }
            undo_function();
            moved=0;
        }
    }
    if(r==2&&used_board[r+2][c]==empty_board[r+2][c]&&used_board[r+1][c]==empty_board[r+1][c])
    {
        moves_hint[0][moves_counter]=r+2;
        moves_hint[1][moves_counter]=c;
        moves_counter++;
        dp = used_board[r+2][c];
        move(c, r, c, r+2, mp,dp);
        check_after=0;
        check(otherteam ,myteam);
        if (check_after==1)
        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
}

void generate_b(int r , int c , char piece , char myteam[5][16] , char otherteam [5][16])
{
    int i , j ;
    if(piece=='b'||piece=='B')
    {
        moves_counter=0;
    }
    char dp;
    char mp = used_board[r][c];
    for (i=1,  j=1 ; (r+i<9&&c+j<9); i++, j++)
    {
        if(used_board[r+i][c+j]==empty_board[r+i][c+j])
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
            generation[r+i][c+j]=1;
        }
        else if ((isupper(used_board[r+i][c+j])&&islower(piece))||(islower(used_board[r+i][c+j])&&isupper(piece)))//take out
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
            generation[r+i][c+j]=1;
            break;
        }
        else
            break;

        dp = used_board[r+i][c+j];
        move(c, r, c+j, r+i, mp,dp);
        check_after=0;
        check(otherteam ,myteam);
        if (check_after==1)

        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
    for (i=-1,  j=-1 ; (r+i>0&&c+j>0); i--, j--)
    {
        if(used_board[r+i][c+j]==empty_board[r+i][c+j])
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
        }
        else if ((isupper(used_board[r+i][c+j])&&islower(piece))||(islower(used_board[r+i][c+j])&&isupper(piece)))
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
            break;
        }
        else
        {
            break;
        }
        dp = used_board[r+i][c+j];
        move(c, r, c+j, r+i, mp,dp);
        check_after=0;
        check(otherteam ,myteam);
        if (check_after==1)

        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
    for (i=1,  j=-1 ; (r+i<9&&c+j>0); i++, j--)
    {
        if(used_board[r+i][c+j]==empty_board[r+i][c+j])
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
        }
        else if ((isupper(used_board[r+i][c+j])&&islower(piece))||(islower(used_board[r+i][c+j])&&isupper(piece)))
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
            break;
        }
        else
        {
            break;
        }
        dp = used_board[r+i][c+j];
        move(c, r, c+j, r+i, mp,dp);
        check_after=0;
        check(otherteam ,myteam);
        if (check_after==1)

        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
    for (i=-1,  j=1 ; (r+i>0&&c+j<9); i--, j++)
    {
        if(used_board[r+i][c+j]==empty_board[r+i][c+j])
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
        }
        else if ((isupper(used_board[r+i][c+j])&&islower(piece))||(islower(used_board[r+i][c+j])&&isupper(piece)))
        {
            moves_hint[0][moves_counter]=r+i;
            moves_hint[1][moves_counter]=c+j;
            moves_counter++;
            break;
        }
        else
        {
            break;
        }
        dp = used_board[r+i][c+j];
        move(c, r, c+j, r+i, mp,dp);
        check_after=0;
        check(otherteam ,myteam);
        if (check_after==1)
        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
}

void generate_r (int r , int c ,char piece,char myteam[5][16] , char otherteam [5][16])
{
    moves_counter=0;
    int k , j;
    char dp;
    char mp = used_board[r][c];
    for (j=c+1 ; j<9 ; j++)
    {
        if(used_board[r][j]==empty_board[r][j])
        {
            moves_hint[0][moves_counter]=r;
            moves_hint[1][moves_counter]=j;
            moves_counter++;
        }
        else if ((isupper(used_board[r][j])&&islower(piece))||(islower(used_board[r][j])&&isupper(piece)))//take out
        {
            moves_hint[0][moves_counter]=r;
            moves_hint[1][moves_counter]=j;
            moves_counter++;
            break;
        }
        else
        {
            break;
        }
        dp = used_board[r][j];
        move(c, r, j, r, mp,dp);
        if (threat==1)
        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
    for(j=c-1 ; j>0 ; j--)
    {
        if(used_board[r][j]==empty_board[r][j])
        {
            moves_hint[0][moves_counter]=r;
            moves_hint[1][moves_counter]=j;
            moves_counter++;
        }
        else if ((isupper(used_board[r][j])&&islower(piece))||(islower(used_board[r][j])&&isupper(piece)))
        {
            moves_hint[0][moves_counter]=r;
            moves_hint[1][moves_counter]=j;
            moves_counter++;
            break;
        }
        else
        {
            break;
        }
        dp = used_board[r][j];
        move(c, r, j, r, mp,dp);
        if (threat==1)
        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
    for(k=r+1 ; k<9; k++)
    {
        if(used_board[k][c]==empty_board[k][c])
        {
            moves_hint[0][moves_counter]=k;
            moves_hint[1][moves_counter]=c;
            moves_counter++;
        }

        else if ((isupper(used_board[k][c])&&islower(piece))||(islower(used_board[k][c])&&isupper(piece)))
        {
            moves_hint[0][moves_counter]=k;
            moves_hint[1][moves_counter]=c;
            moves_counter++;
            break;
        }
        else
            break;

        dp = used_board[k][c];
        move(c, r, c, k, mp,dp);
        if (threat==1)
        {
            moves_counter--;
        }
        undo_function();
        moved=0;
    }
    for(k=r-1 ; k>0; k--)
    {
        if(used_board[k][c]==empty_board[k][c])
        {
            moves_hint[0][moves_counter]=k;
            moves_hint[1][moves_counter]=c;
            moves_counter++;
        }
        else if ((isupper(used_board[k][c])&&islower(piece))||(islower(used_board[k][c])&&isupper(piece)))
        {
            moves_hint[0][moves_counter]=k;
            moves_hint[1][moves_counter]=c;
            moves_counter++;
            break;
        }
        else
            break;

        dp = used_board[k][c];
        move(c, r, c, k, mp,dp);
        if (threat==1)
        {
           moves_counter--;
        }
        undo_function();
        moved=0;
    }
}

void generate_n(int r , int c , char piece, char myteam[5][16] , char otherteam[5][16])
{
    moves_counter=0;
    char mp = used_board[r][c];
    char dp;
    int i , j ;
    for(i=2 , j=1;;)
    {
        if(r+i>0 && r+i<9 && c+j>0 && c+j<9 )
        {
            if(used_board[r+i][c+j]==empty_board[r+i][c+j])
            {
                moves_hint[0][moves_counter]=r+i;
                moves_hint[1][moves_counter]=c+j;
                moves_counter++;
                dp = used_board[r+i][c+j];
                move(c, r, c+j, r+i, mp,dp);
                check_after=0;
                check(otherteam ,myteam);
                if (check_after==1)
                {
                    moves_counter--;
                }
                undo_function();
                moved=0;
            }
            else if ((isupper(used_board[r+i][c+j])&&islower(piece))||(islower(used_board[r+i][c+j])&&isupper(piece)))
            {
                moves_hint[0][moves_counter]=r+i;
                moves_hint[1][moves_counter]=c+j;
                moves_counter++;
                dp = used_board[r+i][c+j];
                move(c, r, c+j, r+i, mp,dp);
                check_after=0;
                check(otherteam ,myteam);
                if (check_after==1)
                {
                    moves_counter--;
                }
                undo_function();
                moved=0;
            }
        }
        if ((i==2 && j==1)|| (i==1 && j==2))
        {
            i=-i;
            j=-j;
        }
        else if (i==-2 && j==-1)
        {
            i=1;
            j=2;
        }
        else if (i==-1 && j==-2)
        {
            j=2;
        }
        else if (i==-1 && j==2)
        {
            i=2 ;
            j=-1;
        }
        else if (i==2 && j==-1)
        {
            i=-i ;
            j=-j;
        }
        else if (i==-2 && j==1)
        {
            i=1 ;
            j=-2;
        }
        else if (i==1 && j==-2)
        {
            break;
        }
    }
}
void generate_k(int r , int c , char piece,char myteam[5][16] , char otherteam [5][16])
{
    char dp;
    int i ,j;
    moves_counter=0;
    for (i=r-1 ; i<r+2 ; i++)
    {
        if(i>0&&i<9)
        {
            for (j=c-1 ; j<c+2 ; j++ )
            {
                if(j>0&&j<9)
                {
                    if (i==r && j==c)
                    {
                        continue;
                    }
                    dp = used_board[i][j];
                    if((isupper(piece)&&islower(dp))||(isupper(dp)&&islower(piece))||dp==empty_board[i][j])
                        move(c, r, j, i, piece,dp);
                    if (!check(otherteam , myteam))
                    {
                        generation[i][j]=1;
                    }
                    undo_function();
                    moved=0;
                }
            }
        }
    }
}
void promotion(int pc , int pr)
{
    int i , j;
    while(1)
    {
        if(propiece!='Q'&& propiece!='B'&& propiece!='R'&& propiece!='N')//right piece
        {
            printf("enter the wanted piece");
            fflush(stdin);
            scanf("%c",&propiece);
        }
        else
        {
            if(abs(playerc)%2==0)
                used_board[pr][pc]=tolower((int)propiece);
            else
                used_board[pr][pc]=propiece;

            break;
        }
    }
    if (abs(playerc)%2==0)//player one is playing
    {
        undo[4][undoc-1]=tolower((int)propiece);
        for (i=0 ; i<16 ; i++)
        {

            if ((int)white_team_used[1][i]-48==pr && (int)white_team_used[2][i]-64==pc)
            {
                white_team_used[0][i]=tolower((int)propiece);
                break;
            }
        }
    }
    else
    {
        undo[4][undoc-1]=(int)propiece;
        for (i=0 ; i<16 ; i++)
        {

            if ((int)black_team_used[1][i]-48==pr && (int)black_team_used[2][i]-64==pc)
            {
                black_team_used[0][i]=propiece;
                break;
            }
        }
    }
}
void valid_p(int pc,int pr,int dc,int dr,char mp,char dp)
{
    if(isupper((int)used_board[dr][dc]) && abs(dc-pc)==1&& pr-dr==1)
    {
        if(dr==1)
        {
            move(pc, pr, dc, dr, mp,  dp);
            promotion(dc,dr);
            if (check(white_team_used,black_team_used))
            {
                printf("check!!!!!!\n");
            }
        }
        else
            move(pc, pr, dc, dr, mp,  dp);
    }
    else if( pr==7 && pc==dc&&dr-pr==-1 &&used_board[pr-1][dc]==empty_board[pr-1][dc])
        move(pc, pr, dc, dr, mp,  dp);

        else if(pr==7 && pc==dc&&dr-pr==-2 &&used_board[pr-2][dc]==empty_board[pr-2][dc])
            move(pc, pr, dc, dr, mp,  dp);

    else if( pr!=7 && pc==dc &&dr-pr==-1 )
    {
        if(dr==1)
        {
            move(pc, pr, dc, dr, mp,  dp);
            promotion(dc,dr);
            if (check(white_team_used,black_team_used))
            {
                printf("check!!!!!!\n");
            }
        }
        else
            move(pc, pr, dc, dr, mp,  dp);
    }
    else
        printf("Invalid move..,Try again\n");
}

void valid_P(int pc,int pr,int dc,int dr,char mp,char dp)//Is it a valid move for P
{
    if(islower((int)used_board[dr][dc]) && abs(dc-pc)==1&& dr-pr==1)
    {
        if(dr==8)
        {
            move(pc, pr, dc, dr, mp,  dp);
            promotion(dc,dr);
            if (check(black_team_used,white_team_used))
            {
                printf("check!!!!!!\n");
            }
        }
        else
        {
            move(pc, pr, dc, dr, mp,  dp);
        }
    }
    else if(pr!=2 && pc==dc && dr-pr==1)
    {
        if(dr==8)
        {
            move(pc, pr, dc, dr, mp,  dp);
            promotion(dc,dr);
            if (check(black_team_used,white_team_used))
            {
                printf("check!!!!!!\n");
            }
        }
        else
        {
            move(pc, pr, dc, dr, mp,  dp);
        }
    }
    else if( pr==2 && pc==dc&&dr-pr==1 &&used_board[pr+1][dc]==empty_board[pr+1][dc])
        move(pc, pr, dc, dr, mp,  dp);

        else if(pr==2 && pc==dc&&dr-pr==2 &&used_board[pr+2][dc]==empty_board[pr+2][dc])
            move(pc, pr, dc, dr, mp,  dp);
    else
        printf("Invalid move,Try again\n");
}

void valid_r(int pc,int pr,int dc,int dr,char mp,char dp)
{
    int flag=1;
    int i,j;

    if(dc == pc && pr != dr)
    {
        if(dr>pr)
        {
            for(i=pr+1; i<dr ; i++)
            {
                if(used_board[i][pc]!=empty_board[i][pc])
                {
                    flag=0;
                    break;
                }
            }
        }
        else
        {
            for(i=pr-1; i>dr ; i--)
            {
                if(used_board[i][pc]!=empty_board[i][pc])
                {
                    flag=0;
                    break;
                }
            }
        }
    }
    else if(dc != pc && pr == dr)
    {
        if(dc>pc)
        {
            for(i=pc+1; i<dc ; i++)
            {
                if(used_board[pr][i]!=empty_board[pr][i])
                {
                    flag=0;
                    break;
                }
            }
        }
        else
        {
            for(i=pc-1; i>dc ; i--)
            {
                if(used_board[pr][i]!=empty_board[pr][i])
                {
                    printf("FOURTH IF");
                    flag=0;
                    break;
                }
            }
        }
    }
    else
        flag=0;

    if(flag)
    {
        move(pc, pr, dc, dr, mp,  dp);
    }
    else
        printf("Invalid move,Try again\n");
}

void valid_n(int pc,int pr,int dc,int dr,char mp,char dp)
{
    if((abs(dc-pc)==1 && abs(dr-pr)==2) || (abs(dc-pc)==2 && abs(dr-pr)==1))
        move(pc, pr, dc, dr, mp,  dp);
    else
        printf("Invalid move,Try again\n");
}

void valid_b(int pc,int pr,int dc,int dr,char mp,char dp)
{
    int flagb=1;
    int i,j;
    if(abs(dc-pc) == abs(dr-pr))
    {
        if(dc>pc && dr<pr) //case 1
        {
            j=pc+1;
            for(i=pr-1; i>dr; i--)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j++;
                }
                else
                {
                    flagb=0;
                    break;
                }
            }
        }
        else if(dc>pc && dr>pr) //case 2
        {
            j=pc+1;
            for(i=pr+1; i<dr; i++)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j++;
                }
                else
                {
                    flagb=0;
                    break;
                }
            }
        }
        else if(dc<pc && dr<pr) //case 3
        {
            j=pc-1;
            for(i=pr-1; i>dr; i--)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j--;
                }
                else
                {
                    flagb=0;
                    break;
                }
            }
        }
        else if(dc<pc && dr>pr) //case 4
        {
            j=pc-1;
            for(i=pr+1; i<dr; i++)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j--;
                }
                else
                {
                    flagb=0;
                    break;
                }
            }
        }
    }
    else
    {
        flagb=0;
    }
    if(flagb)
    {
        move(pc, pr, dc, dr, mp, dp);
    }
    else
        printf("Invalid move, Try again\n");
}

void valid_q(int pc,int pr,int dc,int dr,char mp,char dp)
{
    int flagq=1;
    int i,j;

    if(dc == pc && pr != dr) //like r case 1
    {
        if(dr>pr)
        {
            for(i=pr+1; i<dr ; i++)
            {
                if(used_board[i][pc]!=empty_board[i][pc])//there is a piece in the way
                {
                    flagq=0;
                    break;
                }
            }
        }
        else
        {
            for(i=pr-1; i>dr ; i--)
            {
                if(used_board[i][pc]!=empty_board[i][pc])
                {
                    flagq=0;
                    break;
                }
            }
        }
    }

    else if(dc != pc && pr == dr) //Like r case 2
    {
        if(dc>pc)
        {
            for(i=pc+1; i<dc ; i++)
            {
                if(used_board[pr][i]!=empty_board[pr][i])
                {
                    flagq=0;
                    break;
                }
            }
        }
        else
        {
            for(i=pc-1; i>dc ; i--)
            {
                if(used_board[pr][i]!=empty_board[pr][i])
                {
                    flagq=0;
                    break;
                }
            }
        }
    }

    else if(abs(dc-pc) == abs(dr-pr)) //like b
    {
        if(dc>pc && dr<pr) //case 1
        {
            j=pc+1;
            for(i=pr-1; i>dr; i--)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j++;
                }
                else
                {
                    flagq=0;
                    break;
                }
            }
        }
        else if(dc>pc && dr>pr) //case 2
        {
            j=pc+1;
            for(i=pr+1; i<dr; i++)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j++;
                }
                else
                {
                    flagq=0;
                    break;
                }
            }
        }
        else if(dc<pc && dr<pr) //case 3
        {
            j=pc-1;
            for(i=pr-1; i>dr; i--)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j--;
                }
                else
                {
                    flagq=0;
                    break;
                }
            }
        }
        else if(dc<pc && dr>pr) //case 4
        {
            j=pc-1;
            for(i=pr+1; i<dr; i++)
            {
                if(used_board[i][j]==empty_board[i][j])
                {
                    j--;
                }
                else
                {
                    flagq=0;
                    break;
                }
            }
        }
    }
    else
    {
        flagq=0;
    }
    if(flagq)
    {
        move(pc, pr, dc, dr, mp, dp);
    }
    else
        printf("Invalid move, Try again\n");
}

void valid_k(int pc,int pr,int dc,int dr,char mp,char dp)
{
    if(abs(pc-dc)==1||abs(pr-dr)==1)
    {
        move(pc,pr,dc,dr,mp,dp);
    }
}

int check(char otherteam[5][16],char team[5][16])//Checks if the move is a threat on a king
{
    int i,x=0;
    check_after=0;
    for(i=0; i<16; i++)
    {
        if(otherteam[3][i]=='1')
        {
            switch(otherteam[0][i])
            {
            case 'p':
                x=check_p((int)otherteam[1][i]-48,(int)otherteam[2][i]-64,(int)team[1][4]-48,(int)team[2][4]-64);
                break;
            case 'P':
                x=check_P((int)otherteam[1][i]-48,(int)otherteam[2][i]-64,(int)team[1][4]-48,(int)team[2][4]-64);
                break;
            case 'r':
            case 'R':
                x=check_r((int)otherteam[1][i]-48,(int)otherteam[2][i]-64,(int)team[1][4]-48,(int)team[2][4]-64);
                break;
            case 'N':
            case 'n':
                x=check_n((int)otherteam[1][i]-48,(int)otherteam[2][i]-64,(int)team[1][4]-48,(int)team[2][4]-64);
                break;
            case 'b':
            case 'B':
                x=check_b((int)otherteam[1][i]-48,(int)otherteam[2][i]-64,(int)team[1][4]-48,(int)team[2][4]-64);
                break;
            case 'q':
            case 'Q':
                if ((check_b((int)otherteam[1][i]-48,(int)otherteam[2][i]-64,(int)team[1][4]-48,(int)team[2][4]-64))||(check_r((int)otherteam[1][i]-48,(int)otherteam[2][i]-64,(int)team[1][4]-48,(int)team[2][4]-64)))
                {
                    x=1;
                }
                break;
            }
            if (x)
            {
                check_after=1 ;
                break;
            }
        }
    }
    return check_after;
}
int check_p(int prr, int pcc,int drr,int dcc)
{
    if(abs(dcc-pcc)==1&&drr-prr==-1)
        return 1;
    else
        return 0;

}
int check_P(int prr, int pcc,int drr,int dcc)
{
    if(abs(dcc-pcc)==1&& drr-prr==1)
        return 1;

    else
        return 0;
}
int check_n(int prr,int pcc,int drr,int dcc)
{
    if((abs(dcc-pcc)==1 && abs(prr-drr)==2) || (abs(dcc-pcc)==2 && abs(prr-drr)==1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int check_r(int prr, int pcc,int drr,int dcc)
{
    if(dcc == pcc && prr != drr)
    {
        int i,j=pcc;
        if (drr>prr)
        {
            for(i=prr+1; i<drr; i++)
            {
                if(used_board[i][j]!=empty_board[i][j])
                {
                    return 0;
                }
            }
        }
        else
        {
            for(i=prr-1; i>drr; i--)
            {
                if(used_board[i][j]!=empty_board[i][j])
                {
                    return 0;
                }
            }
        }
    }
    else if(dcc != pcc && prr == drr)
    {
        int i=prr,j=pcc;
        if (dcc>pcc)
        {
            for(j=pcc+1; j<dcc; j++)
            {
                if(used_board[i][j]!=empty_board[i][j])
                {
                    return 0;
                }
            }
        }
        else
        {
            for(j=pcc-1; j>dcc; j--)
            {
                if(used_board[i][j]!=empty_board[i][j])
                {
                    return 0;
                }
            }
        }
    }
    else
    {
        return 0;
    }
    return 1;
}

int check_b(int prr, int pcc,int drr,int dcc)
{
    int i, j;
    if(abs(dcc-pcc) == abs(drr-prr))
    {
        if (dcc>pcc && drr>prr)
        {
            for (i=1,  j=1 ; (prr+i<drr&&pcc+j<dcc); i++, j++)
            {
                if(used_board[prr+i][pcc+j]!=empty_board[prr+i][pcc+j])
                {
                    return 0;
                }
            }
        }
        else if (dcc<pcc && drr<prr)
        {
            for (i=-1,  j=-1 ; (prr+i>drr&&pcc+j>dcc); i--, j--)
            {
                if(used_board[prr+i][pcc+j]!=empty_board[prr+i][pcc+j])
                {
                    return 0;
                }
            }
        }
        else if (drr>prr && dcc<pcc)
        {
            for (i=1,  j=-1 ; (prr+i<drr&&pcc+j>dcc); i++, j--)
            {
                if(used_board[prr+i][pcc+j]!=empty_board[prr+i][pcc+j])
                {
                    return 0;
                }
            }
        }
        else if (drr<prr && dcc>pcc)
        {
            for (i=-1,  j=1 ; (prr+i>drr&&pcc+j<dcc); i--, j++)
            {
                if(used_board[prr+i][pcc+j]!=empty_board[prr+i][pcc+j])
                {
                    return 0;
                }
            }
        }
    }
    else
    {
        return 0;
    }
    return 1;
}
int check_mate(char myteam[5][16],char otherteam[5][16])
{
    int n,m,i,j;
    i=(int)myteam[1][4]-48;
    j=(int)myteam[2][4]-64;

    for(n=i-1; n<=i+1; n++) //1-check the king moves
    {
        for(m=j-1; m<=j+1; m++)
            if(n!=i&&m!=j&&n>0&&m>0&&n<9&&m<9&&used_board[n][m]==empty_board[n][m])
            {
                if(!check(otherteam,myteam))
                    return 0;
            }
            else continue;
    }
    return 1;
}
void move(int pc,int pr,int dc,int dr,char mp, char dp)
{
    int i , j ;
    moved=0;
    if(islower(mp))//white team is playing
    {

        if (islower(dp))
        {
            printf("It's your piece !!\n please enter the piece you want to move");
        }
        else
        {
            moved=1;
            for (i=0 ; i<16 ; i++)
            {

                if ((int)white_team_used[1][i]-48==pr && (int)white_team_used[2][i]-64==pc)
                {
                    white_team_used[1][i]=(char)dr+48 ;
                    white_team_used[2][i]=(char)dc+64 ;
                    break;
                }
            }
            if(dp==empty_board[dr][dc])
            {
                used_board[dr][dc]=mp;
                used_board[pr][pc]=empty_board[pr][pc];
            }
            else if(isupper(dp))
            {
                for (i=0 ; i<16 ; i++)
                {
                    if ((int)black_team_used[1][i]-48==dr && (int)black_team_used[2][i]-64==dc)//////////
                    {
                        black_team_used[3][i]='0';
                        break;
                    }
                }
                undo[5][undoc]=1;  //1 for taken black
                taken_black[taken_blackc]=dp;
                taken_blackc++;
                used_board[dr][dc]=mp;
                used_board[pr][pc]=empty_board[pr][pc];
            }
            undo[0][undoc]=pr;
            undo[1][undoc]=pc;
            undo[2][undoc]=dr;
            undo[3][undoc]=dc;
            undoc++;
            check_after=0;
            check(black_team_used,white_team_used);
            if (check_after==1)
            {
                threat=1;
            }
            else
            {
                check_after=0;
                check(white_team_used,black_team_used);
            }
        }
    }
    else if (isupper(mp))
    {
        if (isupper(dp))
        {
            printf("It's your piece !!\n");
        }
        else
        {
            moved=1;
            for (i=0 ; i<16 ; i++)
            {
                if ((int)black_team_used[1][i]-48==pr && (int)black_team_used[2][i]-64==pc)////////////
                {
                    black_team_used[1][i]=(char)dr+48 ;
                    black_team_used[2][i]=(char)dc+64 ;
                    break;
                }
            }
            if(dp==empty_board[dr][dc])
            {
                used_board[dr][dc]=mp;
                used_board[pr][pc]=empty_board[pr][pc];
            }
            else if(islower(dp))
            {
                for (i=0 ; i<16 ; i++)
                {
                    if ((int)white_team_used[1][i]-48==dr && (int)white_team_used[2][i]-64==dc)////////
                    {
                        white_team_used[3][i]='0';
                        break;
                    }
                }
                undo[5][undoc]=2;  //2 for taken white
                taken_white[taken_whitec]=dp;
                taken_whitec++;
                used_board[dr][dc]=mp;
                used_board[pr][pc]=empty_board[pr][pc];
            }
            undo[0][undoc]=pr;
            undo[1][undoc]=pc;
            undo[2][undoc]=dr;
            undo[3][undoc]=dc;
            undoc++;
            check_after=0;
            check(white_team_used,black_team_used);
            if (check_after==1)
            {
                threat=1;
            }
            else
            {
                check_after=0;
                check(black_team_used,white_team_used);
            }
        }
    }
}

void save_current()
{
    int i , j;
    FILE *fp ;
    fp= fopen("loadgame.txt", "w");
    for(i=0 ; i<9 ; i++)
    {
        for(j=0 ; j<9 ; j++)
        {
            fprintf(fp,"%c",used_board[i][j]);
        }
    }
    for(i=0 ; i<16 ; i++)
    {
        fprintf(fp,"%c",taken_white[i]);
    }
    fprintf(fp,"%d",taken_whitec);
    for(i=0 ; i<16 ; i++)
    {
        fprintf(fp,"%c",taken_black[i]);
    }
    fprintf(fp,"%d",taken_blackc);

    for(i=0 ; i<5 ; i++)
    {
        for(j=0 ; j<16 ; j++)
        {
            fprintf(fp,"%c", white_team_used[i][j]);
        }
    }
    for(i=0 ; i<5 ; i++)
    {
        for(j=0 ; j<16 ; j++)
        {
            fprintf(fp,"%c",black_team_used[i][j]);
        }
    }
    fprintf(fp,"%d",playerc);
    fclose(fp);
}

int main()
{
    int i,j,k;
    printf("Welcome To CHESS FOR FUN\n");
    while(1)
    {
        printf("Choose From The List : \n 1-new game \t 2-load game\n");
        int x ;
        scanf("%d",&x);
        if (x==1)//new game
        {
            for(i=0 ; i<9; i++)
            {
                for( j=0 ; j<9 ; j++)
                {
                    used_board[i][j]=new_board[i][j];
                }
            }
            for(i=0 ; i<5 ; i++)
            {
                for(j=0 ; j<16 ; j++)
                {
                    white_team_used[i][j]= white_team[i][j];
                    black_team_used[i][j]=black_team[i][j];
                }
            }
            break;
        }
        else if(x==2)//load game
        {
            FILE *fp ;
            fp = fopen("loadgame.txt","r");
            for(i=0 ; i<9; i++) //read the board from the file
            {
                for( j=0 ; j<9 ; j++)
                {
                    used_board[i][j]=fgetc(fp);
                }
            }
            for(i=0 ; i<16 ; i++) //read taken pieces from white team
            {
                fscanf(fp,"%c",&taken_white[i]);
            }
            fscanf(fp,"%d",&taken_whitec);
            for(i=0 ; i<16 ; i++) //read taken pieces from black team
            {
                fscanf(fp,"%c",&taken_black[i]);
            }
            fscanf(fp,"%d",&taken_blackc);
            for(i=0 ; i<5 ; i++)
            {
                for(j=0 ; j<16 ; j++) //positions
                {
                    fscanf(fp,"%c",&white_team_used[i][j]);
                }
            }
            for(i=0 ; i<5 ; i++)
            {
                for(j=0 ; j<16 ; j++)
                {
                    fscanf(fp,"%c",&black_team_used[i][j]);
                }
            }

            fscanf(fp,"%d",&playerc); //whose turn
            playerc++;
            fclose(fp);
            break;
        }
        else
        {
            printf("Choose From The List Please");
        }
    }
    char positionC,positionR,destinationC,destinationR;
    system("cls");
    printf("%d\n",playerc);
    for(i=0 ; i<9 ; i++)
    {
        for(j=0 ; j<9 ; j++)
        {
            printf("%c   ",used_board[i][j]);

        }
        if (i>0)
        {
            printf("%d",9-i);
        }
        if (i==7)
        {
            printf("\t");
            for(k=0; k<taken_blackc ; k++)
            {
                printf("%c ",taken_black[k]);
            }
        }
        if(i==1)
        {
            printf("\t");
            for(k=0; k<taken_whitec ; k++)
            {
                printf("%c ",taken_white[k]);
            }
        }
        printf("\n\n");

    }
    while (1)
    {

        if(moved==1)
        {
            playerc++;
            moved=0;
            system("cls"); //clear screen
            printf("%d\n",playerc);
            for(i=0 ; i<9 ; i++)
            {
                for(j=0 ; j<9 ; j++)
                {
                    printf("%c   ",used_board[i][j]);

                }
                if (i>0)
                {
                    printf("%d",9-i);
                }
                if (i==7) // print beside the board
                {
                    printf("\t");
                    for(k=0; k<taken_blackc ; k++)
                    {
                        printf("%c ",taken_black[k]);
                    }
                }
                else if(i==1)  //print beside the board
                {
                    printf("\t");
                    for(k=0; k<taken_whitec ; k++)
                    {
                        printf("%c ",taken_white[k]);
                    }
                }
                else if(i==4&&check_after==1)
                {
                    printf("\t check!!!!");  //beside the board row 4
                    check_after=0;
                }
                printf("\n\n");
            }
        }

        if (playerc%2==1) //player 2
        {
            printf("player two\nPlease Enter A Move\n(z to undo , I to hint , R to redo , S to save , Q to quit): ");
        }
        else
        {
            printf("player one\nPlease Enter A Move\n(z to undo , I to hint , R to redo , S to save , Q to quit): ");
        }
        scanf(" %c",&positionC);
        if (positionC=='q'||positionC=='Q')//quit
        {
            if (playerc%2==0){printf("player 2 wins");}
            else {printf("player one wins");}
            break;
        }
        if (positionC=='S'||positionC=='s')//save
        {
            save_current();
            printf("SAVED\nplease enter a move");
            continue;
        }
        if ((positionC=='z'||positionC=='Z')&&undoc>0)//undo
        {
            redo_flag=1;
            undo_function();
        }
        else if (positionC=='z'||positionC=='Z')
        {
            printf("You Can't undo\n");
        }
        else if ((positionC=='r'||positionC=='R')&&redoc>0)//redo
        {
            redo_function();
        }
        else if (positionC=='r'||positionC=='R')
        {
            printf("You Can't redo\n");
        }
        else if (positionC=='i'||positionC=='I')//hint
        {
            char row , col ;
            printf("enter the piece you want to move");
            while(1)
            {
                scanf(" %c",&col);
                scanf(" %c",&row);
                if(col<'A'|| col>'H'||row<'1'||row>'8')
                {
                    printf("enter a valid piece , please");
                    continue;
                }
                if(playerc%2==0&&isupper(used_board[9-((int)row-48)][(int)col-64]))//if the piece belongs to the player
                {
                    printf("The Piece You Want To Move Is Not Yours\n");
                    continue;
                }
                else if(playerc%2==1&&islower(used_board[9-((int)row-48)][(int)col-64]))//if the piece belongs to the player
                {
                    printf("The Piece You Want To Move Is Not Yours\n");
                    continue;
                }
                break;
            }
            hint(9-((int)row-48),(int)col-64);
        }
        else
        {
            scanf(" %c",&positionR);
            scanf(" %c",&destinationC);
            scanf(" %c",&destinationR);
            scanf("%c",&propiece);
            fflush(stdin);
            if(positionC>='A'&&positionC<='H'&&positionR>='1'&&positionR<='8'&&destinationC>='A'&&
                    destinationC<='H'&&destinationR>='1'&&destinationR<='8'
                    &&(positionC!=destinationC||positionR!=destinationR)) //valid input
            {
                char myPiece = used_board[9-((int)positionR-48)][(int)positionC-64];
                char dpiece = used_board[9-((int)destinationR-48)][(int)destinationC-64];
                if(playerc%2==0&&isupper(used_board[9-((int)positionR-48)][(int)positionC-64]))//Is the piece belongs to the player
                {
                    char x = 7;
                    printf("%c",x);
                    printf("The Piece You Want To Move Is Not Yours\n");
                    continue;
                }
                else if(playerc%2==1&&islower(used_board[9-((int)positionR-48)][(int)positionC-64]))//Is the piece belongs to the player
                {
                    char x = 7;
                    printf("%c",x);
                    printf("The Piece You Want To Move Is Not Yours\n");
                    continue;
                }
                destinationR=9-(int)destinationR+48;
                positionR=9-(int)positionR+48;

                switch(myPiece) //the piece supposed to move
                {
                case 'p':
                    valid_p((int)positionC-64,(int)positionR,(int)destinationC-64,(int)destinationR,myPiece,dpiece);
                    break;
                case'P':
                    valid_P((int)positionC-64,(int)positionR,(int)destinationC-64,(int)destinationR,myPiece,dpiece);
                    break;
                case 'r':
                case'R':
                    valid_r((int)positionC-64,(int)positionR,(int)destinationC-64,(int)destinationR,myPiece,dpiece);
                    break;
                case 'n':
                case'N':
                    valid_n((int)positionC-64,(int)positionR,(int)destinationC-64,(int)destinationR,myPiece,dpiece);
                    break;
                case 'b':
                case'B':
                    valid_b((int)positionC-64,(int)positionR,(int)destinationC-64,(int)destinationR,myPiece,dpiece);
                    break;
                case 'q':
                case'Q':
                    valid_q((int)positionC-64,(int)positionR,(int)destinationC-64,(int)destinationR,myPiece,dpiece);
                    break;
                case 'k':
                case'K':
                    valid_k((int)positionC-64,(int)positionR,(int)destinationC-64,(int)destinationR,myPiece,dpiece);
                    break;
                default:
                    printf("No piece Exists\n");
                    break;
                }
            }
            else //not valid input
            {
                printf("Out of range, Try again\n");
                continue;
            }
            if (threat==1){undo_function();moved=0; threat=0 ;printf("\nthreat on the king\n"); }
        }
        if(moved==1)
        {
            //if (playerc%2==0){possible_moves(white_team_used,black_team_used);}
            //else{possible_moves(black_team_used,white_team_used);}
        }
    }
    printf("\nTHANKS FOR PLAYING ");
    return 0 ;
}
