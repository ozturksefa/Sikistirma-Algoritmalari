#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct huffman
{
    char c;
    int frekans;
    struct huffman *left;
    struct huffman *right;
    struct huffman *next;
    struct huffman *prev;
};

void insert(struct huffman* arr,struct huffman* add){
    struct huffman * gez = arr;
    while (gez->next != NULL) {
        gez = gez->next;
    }

    gez->next = (struct huffman *) malloc(sizeof(struct huffman));
    gez->next->c = add->c;
    gez->next->frekans = add->frekans;
    gez->next->left = NULL;
    gez->next->right = NULL;
    gez->next->next = NULL;
    gez->next->prev = gez;
}

void sort(struct huffman* arr){
    struct huffman * gez = arr;
    while(gez!=NULL){
        int frekans = gez->frekans;
        struct huffman * gez2 = gez->next;
        while(gez2!=NULL){
            if(gez2->frekans > frekans){
                char c = gez2->c;
                frekans = gez2->frekans;
                struct huffman * tmp_left = gez2->left;
                struct huffman * tmp_right = gez2->right;

                gez2->c = gez->c;
                gez2->frekans = gez->frekans;
                gez2->left = gez->left;
                gez2->right = gez->right;
                gez->c = c;
                gez->frekans = frekans;
                gez->left = tmp_left;
                gez->right = tmp_right;
            }
            gez2 = gez2->next;
        }
        gez = gez->next;
    }
}

void list(struct huffman* arr){
    printf("\n");
    struct huffman * gez = arr;
    while(gez!=NULL){
        printf("%c: %d\n",gez->c,gez->frekans);
        gez = gez->next;
    }
}

void getCode(struct huffman* arr,char *prefix, int size_prefix,char c,char *code)
{
    struct huffman * gez = arr;

    if(gez->left==NULL && gez->right==NULL){
        if(c==gez->c){
            prefix[size_prefix] = 0;
            strcpy(code , prefix);
            return prefix;
        }else{
            return "asd";
        }
    }

    if(gez->left){
        prefix[size_prefix++] = '0';
        getCode(gez->left,prefix, size_prefix, c,code);
        size_prefix--;
    }

    if(gez->right){
        prefix[size_prefix++] = '1';
        getCode(gez->right,prefix, size_prefix, c,code);
        size_prefix--;
    }

}

void getTree(struct huffman* arr,char *prefix, int size_prefix)
{
    struct huffman * gez = arr;

    if(gez->left==NULL && gez->right==NULL){
        prefix[size_prefix] = 0;
        printf("Karakter: %c\tFrekans: %d\tHuffman Kodu: %s\n",gez->c,gez->frekans,prefix);
        return;
    }

    if(gez->left){
        prefix[size_prefix++] = '0';
        getTree(gez->left,prefix, size_prefix);
        size_prefix--;
    }

    if(gez->right){
        prefix[size_prefix++] = '1';
        getTree(gez->right,prefix, size_prefix);
        size_prefix--;
    }

}
int main()
{
    char *string = malloc(5000*sizeof(char));
    char satir[1000];
    FILE *lz77_output=fopen("lz77_output.txt","w");
    FILE *deflate_output=fopen("deflate_output.txt","w");
    FILE *fp=fopen("metin.txt","r");
    if(fp=fopen("metin.txt","r")){
        while(!feof(fp)){
            fgets(satir,1000,fp);
            strcat(string,satir);
        }
    }else{
        printf("Aradiginiz Dosya Yok!..");
    }
    fseek(fp, 0, SEEK_END);
    long int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("Metin.txt Input Dosya Boyutu:\t%ld Byte\n",size);
    fclose(fp);

    int i,j,k=1,a;
    // LZ77
    for(i=0;i<strlen(string);i++){
        a=0;
        k=1;
        for(j=0;j<i;j++){
            if(string[i]==string[j] && string[i+k]==string[j+k]){
                fprintf(lz77_output,"[%d,",i-j);
                while(string[i+k]==string[j+k] && (j+k)<i){
                    k++;
                }
                if((i+k)<strlen(string)){
                    fprintf(lz77_output,"%d,C(%c)]",k,string[i+k]);
                    i=i+k;
                    a=1;
                }else{
                    fprintf(lz77_output,"%d,C(%c)]",k-1,string[i+k-1]);
                    i=i+k;
                    a=1;
                }
            }
        }
        if(a==0){
            fprintf(lz77_output,"[0,0,C(%c)]",string[i]);
        }
    }
    fseek(lz77_output, 0, SEEK_END);
    size = ftell(lz77_output);
    fseek(lz77_output, 0, SEEK_SET);
    printf("LZ77 Output Dosya Boyutu:\t%ld Byte\n",size);
    fclose(lz77_output);
    // LZSS
    char *lzssOutput = malloc(5000*sizeof(char));
    for(i=0;i<strlen(string);i++){
        a=0;
        k=1;
        for(j=0;j<i;j++){
            if(string[i]==string[j] && string[i+k]==string[j+k]){
                sprintf(lzssOutput + strlen(lzssOutput),"(%d,",i-j);
                while(string[i+k]==string[j+k] && (j+k)<i){
                    k++;
                }
                if((i+k)<strlen(string)){
                    sprintf(lzssOutput + strlen(lzssOutput),"%d)",k);
                    i=i+k-1;
                    a=1;
                }else{
                    sprintf(lzssOutput + strlen(lzssOutput),"%d)",k);
                    i=i+k-1;
                    a=1;
                }
            }
        }
        if(a==0){
            sprintf(lzssOutput + strlen(lzssOutput),"%c",string[i]);
        }
    }

    //huffman
    struct huffman* arr = NULL;
    arr = (struct huffman*)malloc(sizeof(struct huffman));
    int frekans[255]={0};
    char *harf;
    for (harf = &lzssOutput[0]; *harf != 0; harf++)    {
        frekans[(int)*harf]++;
    }
    a=0;
    for (i = 0; i < 255; i++){
        if(frekans[i]>0){
            if(a==0){
                arr->c = i;
                arr->frekans = frekans[i];
                arr->left = NULL;
                arr->right = NULL;
                arr->next = NULL;
                arr->prev = NULL;
                a=1;
            }else{
                struct huffman* tmp;
                tmp = (struct huffman*)malloc(sizeof(struct huffman));
                tmp->c = i;
                tmp->frekans = frekans[i];
                tmp->left = NULL;
                tmp->right = NULL;
                tmp->next = NULL;
                tmp->prev = NULL;
                insert(arr,tmp);
            }
        }
    }

    sort(arr);

    while (arr->next!=NULL)
    {
        sort(arr);
        //list(arr);
        struct huffman * gez = arr;
        while(gez->next!=NULL){
            gez = gez->next;
        }
        struct huffman *newlast = calloc(1, sizeof(struct huffman));
        newlast->c = gez->c;
        newlast->left = gez;
        newlast->right = gez->prev;
        newlast->next = NULL;
        if(gez->prev->prev!=NULL){
            newlast->prev = gez->prev->prev;
        }else{
            newlast->prev = NULL;
        }
        newlast->frekans = gez->frekans + gez->prev->frekans;
        //printf("%c-%c=>%d\n",gez->c,gez->prev->c,newlast->frekans);
        if(gez->prev->prev!=NULL){
            gez->prev->prev->next = newlast;
        }else{
            arr = newlast;
        }
    }

    char *buffer = malloc(256);
    char *codes = malloc(256);

    for(i=0;i<strlen(lzssOutput);i++){
        getCode(arr,buffer,0,lzssOutput[i],codes);
        fprintf(deflate_output,"%s",codes);
    }

    fseek(deflate_output, 0, SEEK_END);
    size = ftell(deflate_output);
    fseek(deflate_output, 0, SEEK_SET);
    printf("Deflate Output Dosya Boyutu:\t%ld Byte\n\n",size);

    fclose(deflate_output);

    printf("HUFFMAN AGACI\n");
    getTree(arr,buffer,0);

    return 0;
}
