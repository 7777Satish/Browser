#include "parser.h"

void createDOM(char* file_content, Tab** tab) {
    int i = 0;
    // int y = 0;
    
    ItemNode* LIST = NULL;

    int isTag = 0;
    char* currentText = malloc(1);
    strcpy(currentText, "");

    char* currentTagText = malloc(1);
    strcpy(currentTagText, "");

    while (file_content[i] != '\0') {
        char cc = file_content[i];
        if(cc == '\n' || cc == '\t' || (cc==' ' && file_content[i+1] == ' ') || ( i>0 && cc==' ' && file_content[i-1] == '\n')){
            i+=1;
            continue;
        }

        if(cc == '<' && file_content[i+1]!='<' && file_content[i+1]!=' '){
            isTag = 1;
            i+=1;
            if(strlen(currentText)>1){
                // printf("Text: %s\n", currentText);

                // SDL_Color color = {0, 0, 0, 255};
                ItemNode* t = (ItemNode*)(malloc(sizeof(ItemNode)));
                
                t->isText = 1;
                size_t size = strlen(currentText);
                t->content = malloc(size + 1);
                strcpy(t->content, currentText);

                t->next = NULL;
                t->prev = NULL;

                if(!LIST){
                    LIST = t;
                } else {
                    ItemNode* ptr = LIST;
                    while(ptr->next!=NULL){
                        ptr = ptr->next;
                    }
                    t->prev = ptr;
                    ptr->next = t;
                }
            }
            currentText = realloc(currentText, 1);
            currentText[0] = '\0';
            continue;
        }


        if(cc == '>'){
            isTag = 0;
            // printf("<%s>\n", currentTagText);

            // SDL_Color color = {0, 0, 0, 255};
            ItemNode* t = (ItemNode*)(malloc(sizeof(ItemNode)));
            
            size_t size = strlen(currentTagText);
            
            t->isText = 0;
            if(currentTagText[0] == '/'){
                t->isClosing = 1;

                t->content = malloc(size);
                int tIndex = 0;
                for(tIndex = 0; tIndex<size; tIndex++){
                    t->content[tIndex] = currentTagText[tIndex+1];
                }
                t->content[size-1] = '\0';
            } else {
                t->isClosing = 0;

                t->content = malloc(size + 1);
                strcpy(t->content, currentTagText);
            }
            
            t->next = NULL;
            t->prev = NULL;

            if(!LIST){
                LIST = t;
            } else {
                ItemNode* ptr = LIST;
                while(ptr->next!=NULL){
                    ptr = ptr->next;
                }
                t->prev = ptr;
                ptr->next = t;
            }


            currentTagText = realloc(currentTagText, 1);
            currentTagText[0] = '\0';
            i+=1;
            continue;
        }

        if(!isTag){
            size_t t = strlen(currentText);
            char* newText = realloc(currentText, t+2);
            
            currentText = newText;
            currentText[t] = cc;
            currentText[t+1] = '\0';
        } else {
            size_t t = strlen(currentTagText);
            char* newTagText = realloc(currentTagText, t+2);
            
            currentTagText = newTagText;
            currentTagText[t] = cc;
            currentTagText[t+1] = '\0';
        }

        i++;
    }

    ItemNode* node = LIST;

    TagNode* list = NULL;
    TagNode* currentParent = list;
    while(node!=NULL){
        
        TagNode* temp = (TagNode*)malloc(sizeof(TagNode));
        temp->child = NULL;
        temp->lastChild = NULL;
        temp->next = NULL;
        temp->prev = NULL;
        temp->parent = currentParent;
        temp->t1 = NULL;
        temp->isText = node->isText;
        // if(node->name){
        //     size_t size = strlen(node->name);
        //     temp->name = malloc(size+1);
        //     strcpy(temp->name, node->name);
        // }

        if(node->content){
            size_t size = strlen(node->content);
            temp->content = malloc(size+1);
            strcpy(temp->content, node->content);

            parseTag(temp);
        }

        if(list == NULL){
            list = temp;

            node = node->next;
            currentParent = temp;
            continue;
        }

        if(node->isText){
            if(!currentParent->child){
                currentParent->child = temp;
            } else {
                TagNode* abc = currentParent->child;
                while (abc->next != NULL)
                {
                    abc = abc->next;
                }
                abc->next = temp;
            }
            
            temp->parent = currentParent;
            
            node = node->next;
            continue;
        }

        if(!node->isClosing){
            // printf("%s, %s\n", currentParent ? currentParent->content : "NULL", temp->content);
            
            temp->parent = currentParent;

            if(!currentParent->child){
                currentParent->child = temp;
            } else {
                TagNode* abc = currentParent->child;
                while (abc->next != NULL) {
                    abc = abc->next;
                }
                abc->next = temp;
            }

            currentParent = temp;
        } else {
            currentParent = currentParent->parent;
        }

        node = node->next;
    }
    
    free(currentText);
    free(currentTagText);

    (*tab)->DOM = list;
}

void parseTag(TagNode* tag){
    int i = 0;
    while(tag->content[i]){
        char a = tag->content[i];
        printf("%c", a);
        i++;
    }
    printf("\n");
}

void createCSOM(char* file_content, Tab** tab){
    
}

void renderDOM(Tab* tab){
    if(!tab->DOM) return;

    int j = 0;
    renderTag(&tab->DOM, &j, &tab);
    
}

void renderTag(TagNode** tag, int* j, Tab** tab){
    if(!(*tag)) return;

    TagNode* ptr = *tag;
    
    while(ptr!=NULL){

        if(!strcmp(ptr->content, "style")){
            ptr = ptr->next;
            continue;
        }

        if(!strcmp(ptr->content, "title")){
            (*tag)->name = ptr->child->content;
            SDL_Surface* s1 = TTF_RenderText_Blended(poppins_bold, (*tag)->name, tab_fg);
            (*tab)->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_FreeSurface(s1);
            ptr = ptr->next;
            continue;
        }

        if(ptr->content){
            if(!ptr->t1){
                SDL_Color color = {0, 0, 0, 255};
                SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, ptr->content, color);
                ptr->t1 = SDL_CreateTextureFromSurface(renderer, s1);
                SDL_FreeSurface(s1);
            }
            if(ptr->isText){
                int w,h;
                SDL_QueryTexture(ptr->t1, NULL, NULL, &w, &h);
                // printf("%s, %d\n", ptr->content, w);
                SDL_Rect r1 = {
                    BORDER_PADDING,
                    BORDER_HEIGHT * 2 + BORDER_PADDING + h*(*j),
                    w,
                    h
                };
    
                SDL_RenderCopy(renderer, ptr->t1, NULL, &r1);
                (*j)+=1;
            }

        }
        
        if(ptr->child){
            renderTag(&ptr->child, j, tab);
        }

        ptr = ptr->next;
    }
}