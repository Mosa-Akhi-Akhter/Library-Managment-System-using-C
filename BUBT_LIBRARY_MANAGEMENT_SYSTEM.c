#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

typedef struct
{
    char username[15];
    char password[15];
} User;

typedef struct
{
    int id; // must be unique
    char title[50];
    char author[50]; // if more than two, separate using ,(COMMA)
    char ISBN[50];
    char category[50];
    char publication[50];
    char description[255];
    int taken;
} Book;

char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s+strlen(s)-1; (ptr >=s ) && isspace(*ptr); --ptr);
        ptr[1] = '\0';
    return s;
}

int main()
{
    char welcome_option,option, admin_option, username[15], password[15], edit_option, search_option, book_title[50];
    int first_time, c, i, j, id, found, titlecmp;
    FILE *f;
    User user;
    Book book;

    f = fopen("librarian.check", "r");
    if (f == NULL)
    {
        fclose(f);
        // write librarian.check file
        f = fopen("librarian.check", "w");
        fputc(1, f); // write value 1 in the librarian.check
        fclose(f);
    }
    else
    {
        fclose(f);
    }

    // Check Book Id
    f = fopen("id.check", "r");
    if (f == NULL)
    {
        fclose(f);
        f = fopen("id.check", "w");
        fputc(1, f); // Write 1 in the id.check file
        fclose(f);
    }
    else
    {
        fclose(f);
    }


    printf("Welcome to our Library Management System!! \n");
    printf("1: To Show Library All Books\n");
    printf("2: Login as Librarian or Student\n");
    printf("3: Exit\n");
    printf("Enter your choice: ");
    scanf("%c", &welcome_option);

    switch(welcome_option)
    {
    //To see library all books
    case'1':
        f = fopen("book.txt", "rb");
        if (f == NULL){
            printf("\nSorry!! No book record found in the database. First you need to add new book.\n");
            exit(0);
        }else{
            // show all book list
            printf("Book list showing below:\n");
            while(fread(&book, sizeof(Book), 1, f))
            {
                printf("\n\tBook Id: %d\n\t", book.id);
                printf("Title: %s\t", book.title);
                printf("Author: %s\t", book.author);
                printf("ISBN: %s\t", book.ISBN);
                printf("Category: %s\t", book.category);
                printf("Publication: %s\t", book.publication);
                printf("Description: %s\t\n", book.description);
            }
            fclose(f);
        }
        break;

    //Log in option
    case'2':
        printf("Login as:\n");
        printf("1: Librarian\n");
        printf("2: Student\n");
        printf("Enter your choice: ");
        scanf(" %c", &option);

        // Using for check Librarian/Student Option
        switch(option)
        {
            case '1':
            f = fopen("librarian.check", "r");
            if (f == NULL){
                printf("Couldn't read file\n");
                exit(0);
            }else{
                first_time = fgetc(f);
                if (first_time == 1){
                    fclose(f);
                    flush();
                    printf("Provide username and password to setup librarian account:\n");
                    printf("Username (14 characters max): ");
                    fgets(user.username, 15, stdin);
                    printf("Password (14 characters max): ");
                    fgets(user.password, 15, stdin);

                    // write this credential to file
                    f = fopen("credential.bin", "wb");
                    if (f == NULL)
                    {
                        printf("Something went wrong!!\n");
                        exit(0);
                    }
                    fwrite(&user, sizeof(User), 1, f); // write username & password in credential.bin file
                    fclose(f);
                    printf("Exit and login again to continue.\n");
                    f = fopen("librarian.check", "w");
                    fputc(0, f);
                    fclose(f);
                }else{
                    fclose(f);
                    flush();
                    printf("Provide username & password to login\n");
                    printf("Username: ");
                    fgets(username, 15, stdin);
                    printf("Password: ");
                    fgets(password, 15, stdin);

                    // read the credential from file
                    f = fopen("credential.bin", "rb");
                    if (f == NULL)
                    {
                        printf("Something went wrong!!\n");
                        exit(0);
                    }
                    fread(&user, sizeof(User), 1, f);
                    if (strcmp(username, user.username) != 0 || strcmp(password, user.password) != 0)
                    {
                        printf("Username or password invalid\n");
                        exit(0);
                    }
                    printf("You have been successfully login!!\n");
                    printf("1: Add Book\n");
                    printf("2: Search Book\n");
                    printf("3: Delete Book\n");
                    printf("4: Show Book List\n");
                    //printf("5: Edit Book\n");
                    printf("Enter your choice: ");
                    scanf("%c", &admin_option);
                    switch(admin_option)
                    {
                    case '1':
                        flush();
                        printf("Provide the following information\n");
                        printf("Title: ");
                        fgets(book.title, 50, stdin);
                        printf("Author: ");
                        fgets(book.author, 50, stdin);
                        printf("ISBN: ");
                        fgets(book.ISBN, 50, stdin);
                        printf("Category: ");
                        fgets(book.category, 50, stdin);
                        printf("Publication: ");
                        fgets(book.publication, 50, stdin);
                        printf("Description: ");
                        fgets(book.description, 50, stdin);
                        book.taken = 0;

                        // increment ID
                        f = fopen("id.check", "r");
                        if (f == NULL)
                        {
                            printf("Something went wrong!!\n");
                            exit(1);
                        }
                        id = fgetc(f);
                        fclose(f);
                        // Assign Book Id
                        book.id = id;

                        // save the record
                        f = fopen("book.txt", "ab");
                        fwrite(&book, sizeof(Book), 1, f);
                        fclose(f);

                        id++;
                        f = fopen("id.check", "w");
                        fputc(id, f);
                        fclose(f);

                        printf("Book insertion has been successfully completed!!\n");
                        break;
                    case '2':
                        printf("Enter the book id to search: ");
                        scanf("%d", &id);

                        // search in the database
                        f = fopen("book.txt", "rb");
                        found = 0;
                        while(fread(&book, sizeof(Book), 1, f))
                        {
                            if (book.id == id)
                            {
                                // matched
                                found = 1;
                                printf("Your search book found!!\n");
                                printf("Title: %s\n", book.title);
                                printf("Author: %s\n", book.author);
                                printf("ISBN: %s\n", book.ISBN);
                                printf("Category: %s\n", book.category);
                                printf("Publication: %s\n", book.publication);
                                printf("Description: %s\n", book.description);
                                break;
                            }
                        }
                        if (found == 0)
                        {
                            printf("Sorry!! The book is not in the database.\n");
                        }
                        fclose(f);
                        break;

                    case '3':
                        printf("Enter the book id to delete: ");
                        scanf("%d", &id);
                        f = fopen("book.txt", "rb");
                        found = 0;
                        while(fread(&book, sizeof(Book), 1, f)){
                            if (book.id == id)
                            {
                                // matched
                                found = 1;
                                break;
                            }
                        }
                        fclose(f);
                        if (found == 0)
                        {
                            printf("Sorry!! The book is not in the database\n");
                        }
                        else
                        {
                            // create a temporary file
                            FILE * temp;
                            temp = fopen("book.temp", "ab");
                            if (temp == NULL)
                            {
                                printf("Something went wrong\n");
                                exit(1);
                            }

                            f = fopen("book.txt", "rb");

                            // copy all the items except item to delete
                            // to temporary file
                            while(fread(&book, sizeof(Book), 1, f))
                            {
                                if (book.id != id)
                                    fwrite(&book, sizeof(Book), 1, temp);
                            }
                            fclose(f);
                            fclose(temp);

                            // delete original file
                            remove("book.txt");

                            // rename the temporary file
                            rename("book.temp", "book.txt");
                            printf("Book record deleted from the database!!\n");
                        }
                        break;
                    case '4':
                        // open book file
                        f = fopen("book.txt", "rb");
                        if (f == NULL){
                            printf("\nSorry!! No book record found in the database. First you need to add new book.\n");
                            exit(0);
                        }else{
                            // show all book list
                            printf("Book list showing below:\n");
                            while(fread(&book, sizeof(Book), 1, f))
                            {
                                printf("\n\tBook Id: %d\n\t", book.id);
                                printf("Title: %s\t", book.title);
                                printf("Author: %s\t", book.author);
                                printf("ISBN: %s\t", book.ISBN);
                                printf("Category: %s\t", book.category);
                                printf("Publication: %s\t", book.publication);
                                printf("Description: %s\t\n", book.description);
                            }
                            fclose(f);
                        }
                        break;

                    }
                }
            }
            break;

        case '2':
            // open book file
            f = fopen("book.txt", "rb");
            if (f == NULL){
                printf("\nSorry!! No book record found in the database. Please contact with librarian.\n");
                exit(0);
            }else{
                // show all book list
                printf("Book list showing below:\n");
                while(fread(&book, sizeof(Book), 1, f))
                {
                    printf("\nBook Id: %d *** ", book.id);
                    printf("Book Title: %s", book.title);

                }
                fclose(f);

                //search options
                printf("\nWhich field do you want to search:\n");
                printf("1. Id\n");
                printf("2. Title\n");
                printf("Enter your search choice: ");
                scanf("\n%c", &search_option);

                // search in the database
                f = fopen("book.txt", "rb");
                found = 0;
                i = 0;
                switch(search_option)
                {
                    case '1':

                        printf("Enter the book id to search: ");
                        scanf("%d", &id);

                        while(fread(&book, sizeof(Book), 1, f))
                        {
                            printf("%d==%d\n",book.id, id);
                            if (book.id == id)
                            {
                                // matched
                                found = 1;
                                printf("Your search book found!!\n");
                                printf("Title: %s\n", book.title);
                                printf("Author: %s\n", book.author);
                                printf("ISBN: %s\n", book.ISBN);
                                printf("Category: %s\n", book.category);
                                printf("Publication: %s\n", book.publication);
                                printf("Description: %s\n", book.description);
                                printf("Status: ");
                                if (book.taken == 1)
                                {
                                    printf("Not Available");
                                }
                                else
                                {
                                    printf("Available\n");
                                }
                                break;
                            }
                            i++;
                        }
                    break;
                    case '2':

                        printf("Enter the book title to search: ");
                        scanf("%s", &book_title);
                        //printf("Input T: %s\n", title);
                        while(fread(&book, sizeof(Book), 1, f))
                        {
                            //printf("%s==%s\n",trim(book.title), book_title);
                            titlecmp = strcmp((trim(book.title)), book_title);
                            if (titlecmp==0)
                            {
                                // matched
                                found = 1;
                                printf("Your search book found!!\n");
                                printf("Title: %s\n", book.title);
                                printf("Author: %s\n", book.author);
                                printf("ISBN: %s\n", book.ISBN);
                                printf("Category: %s\n", book.category);
                                printf("Publication: %s\n", book.publication);
                                printf("Description: %s\n", book.description);
                                printf("Status: ");
                                if (book.taken == 1)
                                {
                                    printf("Not Available");
                                }
                                else
                                {
                                    printf("Available\n");
                                }
                                break;
                            }
                            i++;
                        }
                    break;
                }

                if (found == 0)
                {
                    printf("Sorry!! The book is not in the database.\n");
                }
                fclose(f);
            }
        }
        break;
    case '3':
        printf("Bye Bye!!\n");
         exit(0);
    default:
        printf("Enter either 1 or 2 option only.\n");
        break;
    }
    return 0;
}

