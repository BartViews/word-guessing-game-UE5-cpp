#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();
}
void UBullCowCartridge::SetupGame()
{
    Isograms = GetValidWords(HiddenWordList);
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lifes = HiddenWord.Len()*2;
    PrintLine(TEXT("Welcome to Bull Cows! Press Tab to type."));
    PrintLine(TEXT("\nGuess the %i letter word and press Enter."), HiddenWord.Len());
    //PrintLine(TEXT("The no. of words is: %i"), GetValidWords(HiddenWordList).Num()); //debug line
    //PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); //debug line
}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString Word : HiddenWordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8)
            {
                ValidWords.Emplace(Word);
            }
    }
    return ValidWords;
}
void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (Input == HiddenWord)
    {
        PrintLine(TEXT("You Win!"));
        bGameOver = true;
        EndGame();
    }
    else
    {
        --Lifes;
        IsIsogram(Input);
        int32 Bulls, Cows;
        GetBullCows(Input, Bulls, Cows);
        if (Input.Len() == HiddenWord.Len())
        {
            PrintLine(TEXT("You have %i Bulls and %i Cows."), Bulls, Cows);
        }
        if (Input.Len() == HiddenWord.Len())
        {
            PrintLine(TEXT("This is not the correct word. Try again"));
            PrintLine(TEXT("You have %i lifes left."), Lifes);
        }
        else if (Input.Len() < HiddenWord.Len())
        {
            PrintLine(TEXT("Your guess is too short."));
            PrintLine(TEXT("You have %i lifes left."), Lifes);
        }
        else if (Input.Len() > HiddenWord.Len())
        {
            PrintLine(TEXT("Your guess is too long."));
            PrintLine(TEXT("You have %i lifes left."), Lifes);
        }
        PrintLine(TEXT("Try again."));
    }
    if (Lifes == 0)
    {
        bGameOver = false;
        EndGame();
    }
}
void UBullCowCartridge::EndGame()
{
    if (bGameOver == false)
    {
        ClearScreen();
        PrintLine(TEXT("You have lost. The hidden word was: %s.\nThe game has reset.\n"), *HiddenWord);
        SetupGame();
    }
    else if (bGameOver == true)
    {
        ClearScreen();
        PrintLine(TEXT("You win! Start again.\n"));
        SetupGame();
    }
}
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                PrintLine(TEXT("This is not an isogram."));
                return false;
            }
        }
    }
    return true;
}
void UBullCowCartridge::GetBullCows(const FString& Input, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;
    for (int32 GuessIndex = 0; GuessIndex < Input.Len(); GuessIndex++)
    {
        if (Input.Len() > HiddenWord.Len())
        {
            break;
        }
        
        if (Input[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }
        for (int32 RestIndex = 0; RestIndex < HiddenWord.Len(); RestIndex++)
        {
            if (Input[GuessIndex] == HiddenWord[RestIndex])
            {
                CowCount++;
                break;
            }
        }
    }
}