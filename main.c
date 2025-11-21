#include <stdio.h>
#include <stdlib.h>
#include "survey.h"

int main()
{
    SurveyNode *surveyList = NULL;
    int choice;

    while (1)
    {
        printf("\n=== SURVEY SOFTWARE ===\n");
        printf("1. Create new survey\n");
        printf("2. Add question to existing survey\n");
        printf("3. Delete a question\n");
        printf("4. Delete a survey\n");
        printf("5. View survey and all its questions\n");
        printf("6. Conduct a survey\n");
        printf("7. Publish survey results\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Enter a number.\n");
            scanf("%*[^\n]");
            scanf("%*c");
            continue;
        }

        scanf("%*c"); // consume newline

        switch (choice)
        {
        case 1:
            addSurveyName(&surveyList);
            break;
        case 2:
        {
            SurveyNode *s = selectAnySurvey(surveyList);
            if (s)
                addQuestionToSurvey(s);
            break;
        }
        case 3:
            deleteQuestion(surveyList);
            break;
        case 4:
            deleteSurvey(&surveyList);
            break;
        case 5:
            viewSurveyDetails(surveyList);
            break;
        case 6:
            conductSurvey(surveyList);
            break;
        case 7:
            publishResults(surveyList);
            break;
        case 8:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }
}
