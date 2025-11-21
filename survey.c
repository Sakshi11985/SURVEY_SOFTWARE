#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "survey.h"

BSTNode* createBSTNode(char *option) {
    BSTNode *node = malloc(sizeof(BSTNode));
    strcpy(node->option, option);
    node->count = 1;
    node->left = node->right = NULL;
    return node;
}

BSTNode* insertBST(BSTNode *root, char *option) {
    if (!root) return createBSTNode(option);

    int cmp = strcmp(option, root->option);
    if (cmp == 0) root->count++;
    else if (cmp < 0) root->left = insertBST(root->left, option);
    else root->right = insertBST(root->right, option);

    return root;
}

BSTNode* searchBST(BSTNode *root, const char *key) {
    if (!root) return NULL;
    int cmp = strcmp(key, root->option);
    if (cmp == 0) return root;
    return (cmp < 0) ? searchBST(root->left, key) : searchBST(root->right, key);
}

int totalResponsesBST(BSTNode *root) {
    if (!root) return 0;
    return root->count + totalResponsesBST(root->left) + totalResponsesBST(root->right);
}

SurveyNode* createSurveyNode(char *title) {
    SurveyNode *s = malloc(sizeof(SurveyNode));
    strcpy(s->title, title);
    s->questions = NULL;
    s->conducted = 0;
    s->next = NULL;
    return s;
}

Question* newQuestion(char *text, int numOptions) {
    Question *q = malloc(sizeof(Question));
    strcpy(q->text, text);
    q->numOptions = numOptions;
    q->responses = NULL;
    q->totalResponses = 0;
    q->next = NULL;
    return q;
}

/* ================== ADD SURVEY NAME ================== */

void addSurveyName(SurveyNode **head) {
    char title[100];

    while (1) {
        printf("Enter Survey Title: ");

        int valid = scanf(" %99[^\n]", title);
        scanf("%*c");

        if (valid != 1) {
            printf("Survey title cannot be empty. Enter valid one.\n");
            continue;
        }

        int allSpaces = 1;
        for (int i = 0; title[i]; i++) {
            if (title[i] != ' ' && title[i] != '\t') {
                allSpaces = 0;
                break;
            }
        }

        if (allSpaces) {
            printf("Survey title cannot be empty. Enter valid one.\n");
            continue;
        }

        for (SurveyNode *temp = *head; temp; temp = temp->next) {
            if (strcmp(temp->title, title) == 0) {
                printf("Survey already exists. Enter a different title.\n");
                goto repeat;
            }
        }

        SurveyNode *node = createSurveyNode(title);
        node->next = *head;
        *head = node;
        printf("Survey \"%s\" created!\n", title);
        break;

        repeat: continue;
    }
}

/* ================== SELECT SURVEYS ================== */

SurveyNode* selectAnySurvey(SurveyNode *head) {
    if (!head) {
        printf("No surveys available.\n");
        return NULL;
    }

    int i = 1;
    for (SurveyNode *t = head; t; t = t->next)
        printf("%d. %s\n", i++, t->title);

    int choice;
    printf("Enter survey number: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid number.\n");
        scanf("%*[^\n]");
        scanf("%*c");
        return NULL;
    }
    scanf("%*c");

    int idx = 1;
    for (SurveyNode *t = head; t; t = t->next, idx++)
        if (idx == choice) return t;

    printf("Invalid choice.\n");
    return NULL;
}

/* ================== ADD QUESTIONS ================== */

void addQuestionToSurvey(SurveyNode *s) {
    int qCount;

    while (1) {
        printf("Enter number of questions: ");
        if (scanf("%d", &qCount) != 1) {
            printf("Invalid input. Enter a number.\n");
            scanf("%*[^\n]");
            scanf("%*c");
            continue;
        }
        scanf("%*c");

        if (qCount <= 0) {
            printf("Number of questions cannot be zero or negative.\n");
            continue;
        }

        break;
    }

    for (int k = 1; k <= qCount; k++) {
        char text[200];
        int nopt;

        while (1) {
            printf("\nEnter text for Question %d: ", k);
            int valid = scanf(" %199[^\n]", text);
            scanf("%*c");

            if (valid != 1 || strlen(text) == 0) {
                printf("Question text cannot be empty. Enter valid one.\n");
                continue;
            }
            break;
        }

        while (1) {
            printf("Enter number of options (2-5): ");

            if (scanf("%d", &nopt) != 1) {
                printf("Invalid input. Enter a number.\n");
                scanf("%*[^\n]");
                scanf("%*c");
                continue;
            }
            scanf("%*c");

            if (nopt < 2 || nopt > 5) {
                printf("Number must be between 2 and 5.\n");
                continue;
            }

            break;
        }

        Question *q = newQuestion(text, nopt);

        for (int i = 0; i < nopt; i++) {
            printf("Option %d: ", i + 1);
            scanf(" %49[^\n]", q->options[i]);
            scanf("%*c");
        }

        if (!s->questions) s->questions = q;
        else {
            Question *t = s->questions;
            while (t->next) t = t->next;
            t->next = q;
        }
    }
}

/* ================== VIEW QUESTIONS ================== */

void viewSurveyDetails(SurveyNode *head) {
    SurveyNode *s = selectAnySurvey(head);
    if (!s) return;

    printf("\nSurvey: %s\n", s->title);
    for (Question *q = s->questions; q; q = q->next) {
        printf("\n%s\n", q->text);
        for (int i = 0; i < q->numOptions; i++)
            printf("%d. %s\n", i+1, q->options[i]);
    }
}

/* ================== CONDUCT ================== */

void conductSurvey(SurveyNode *head) {
    SurveyNode *s = selectSurveyWithQuestions(head);
    if (!s) return;

    for (Question *q = s->questions; q; q = q->next) {
        printf("\n%s\n", q->text);
        for (int i = 0; i < q->numOptions; i++)
            printf("%d. %s\n", i+1, q->options[i]);

        int choice;
        printf("Enter choice: ");
        scanf("%d", &choice);
        scanf("%*c");

        q->responses = insertBST(q->responses, q->options[choice-1]);
    }

    s->conducted = 1;
}

/* ================== PUBLISH RESULTS ================== */

SurveyNode* selectSurveyWithQuestions(SurveyNode *head) {
    int i = 0;
    for (SurveyNode *t = head; t; t = t->next)
        if (t->questions) printf("%d. %s\n", ++i, t->title);

    if (i == 0) {
        printf("No surveys contain questions.\n");
        return NULL;
    }

    int choice;
    printf("Select survey: ");
    scanf("%d", &choice);
    scanf("%*c");

    int idx = 0;
    for (SurveyNode *t = head; t; t = t->next)
        if (t->questions && ++idx == choice)
            return t;

    printf("Invalid survey.\n");
    return NULL;
}

void publishResults(SurveyNode *head) {
    SurveyNode *s = selectSurveyWithQuestions(head);
    if (!s) return;

    printf("\nResults for %s\n", s->title);
    for (Question *q = s->questions; q; q = q->next) {
        int total = totalResponsesBST(q->responses);
        printf("\n%s\n", q->text);

        for (int i = 0; i < q->numOptions; i++) {
            BSTNode *node = searchBST(q->responses, q->options[i]);
            int count = node ? node->count : 0;
            float pct = (total == 0) ? 0 : (count * 100.0f / total);

            printf("%s : %d (%.1f%%)\n", q->options[i], count, pct);
        }
    }
}

/* ================== DELETE QUESTION ================== */

void deleteQuestion(SurveyNode *head) {
    SurveyNode *s = selectAnySurvey(head);
    if (!s || !s->questions) {
        printf("No questions to delete.\n");
        return;
    }

    int qno = 1;
    for (Question *q = s->questions; q; q = q->next)
        printf("%d. %s\n", qno++, q->text);

    int choice;
    printf("Enter question number: ");
    scanf("%d", &choice);
    scanf("%*c");

    Question *curr = s->questions, *prev = NULL;
    for (int i = 1; curr && i < choice; i++) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        printf("Invalid question.\n");
        return;
    }

    if (!prev) s->questions = curr->next;
    else prev->next = curr->next;

    free(curr);
    printf("Question deleted.\n");
}

/* ================== DELETE SURVEY ================== */

void deleteSurvey(SurveyNode **head) {
    SurveyNode *s = selectAnySurvey(*head);
    if (!s) return;

    SurveyNode *curr = *head, *prev = NULL;

    while (curr && curr != s) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) return;

    if (!prev) *head = curr->next;
    else prev->next = curr->next;

    Question *q = curr->questions;
    while (q) {
        Question *tmp = q;
        q = q->next;
        free(tmp);
    }

    free(curr);
    printf("Survey deleted.\n");
}
