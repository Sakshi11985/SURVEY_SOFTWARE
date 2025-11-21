#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "survey.h"

// ================= BST SECTION =================

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

// ================= SURVEY STRUCTS =================

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

// ================= ADD SURVEY NAME (SCANF ONLY) =================

void addSurveyName(SurveyNode **head) {
    char title[100];

    while (1) {
        printf("Enter Survey Title: ");
        scanf(" %99[^\n]", title);  // read full line

        // empty?
        if (strlen(title) == 0) {
            printf("Survey title cannot be empty.\n");
            continue;
        }

        // check whitespace-only
        int allSpaces = 1;
        for (int i = 0; title[i]; i++) {
            if (title[i] != ' ' && title[i] != '\t') {
                allSpaces = 0;
                break;
            }
        }
        if (allSpaces) {
            printf("Survey title cannot be empty.\n");
            continue;
        }

        // duplicate?
        for (SurveyNode *temp = *head; temp; temp = temp->next) {
            if (strcmp(temp->title, title) == 0) {
                printf("Survey already exists.\n");
                goto repeat;
            }
        }

        // insert new survey
        SurveyNode *node = createSurveyNode(title);
        node->next = *head;
        *head = node;

        printf("Survey \"%s\" created!\n", title);
        break;

        repeat: continue;
    }
}

// ================= SELECT SURVEY =================

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
    scanf("%d", &choice);
    scanf("%*c");

    int idx = 1;
    for (SurveyNode *t = head; t; t = t->next, ++idx)
        if (idx == choice) return t;

    printf("Invalid choice.\n");
    return NULL;
}

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

    printf("Invalid.\n");
    return NULL;
}

// ================= ADD QUESTIONS =================

void addQuestionToSurvey(SurveyNode *s) {
    int qCount;
    printf("Enter number of questions: ");
    scanf("%d", &qCount);
    scanf("%*c");

    for (int k = 1; k <= qCount; k++) {
        char text[200];
        int nopt;

        printf("\nEnter text for Question %d: ", k);
        scanf(" %199[^\n]", text);

        printf("Enter number of options (2-5): ");
        scanf("%d", &nopt);
        scanf("%*c");

        Question *q = newQuestion(text, nopt);

        for (int i = 0; i < nopt; i++) {
            printf("Option %d: ", i + 1);
            scanf(" %49[^\n]", q->options[i]);
        }

        // append
        if (!s->questions) s->questions = q;
        else {
            Question *t = s->questions;
            while (t->next) t = t->next;
            t->next = q;
        }
    }
}

// ================= VIEW =================

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

// ================= CONDUCT SURVEY =================

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

// ================= PUBLISH RESULTS =================

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

// ================= DELETE QUESTION =================

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

// ================= DELETE SURVEY =================

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

    // free questions
    Question *q = curr->questions;
    while (q) {
        Question *tmp = q;
        q = q->next;
        free(tmp);
    }

    free(curr);
    printf("Survey deleted.\n");
}
