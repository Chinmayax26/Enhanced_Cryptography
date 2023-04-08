#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Cipher class to perform encryption and decryption
class Cipher
{
private:
    string key;

public:
    Cipher(string k)
    {
        key = k;
    }
    string encrypt(string plaintext) // member func for encryption in vignere cipher
    {
        string ciphertext;
        int j = 0;
        for (int i = 0; i < plaintext.size(); i++)
        {
            char c = plaintext[i];
            if (isalpha(c))
            {
                c = toupper(c);
                ciphertext += ((c + key[j]) % 26) + 'A';
                j = (j + 1) % key.length();
            }
        }
        return ciphertext;
    }
    string decrypt(string ciphertext)
    {
        string plaintext;
        int j = 0;
        for (int i = 0; i < ciphertext.size(); i++)
        {
            char c = ciphertext[i];
            if (isalpha(c))
            {
                c = toupper(c);
                plaintext += ((c - key[j] + 26) % 26) + 'A';
                j = (j + 1) % key.length();
            }
        }
        return plaintext;
    }
};

// Genetic algorithm class to optimize the key
class GeneticAlgorithm
{
private:
    int POPULATION_SIZE = 100;
    int MAX_GENERATIONS = 50;
    int TOURNAMENT_SIZE = 5;
    double MUTATION_RATE = 0.1;
    string TARGET = "HELLO";
    string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

public:
    GeneticAlgorithm() {}

    string generateRandomKey(int length)
    {
        string key = "";
        for (int i = 0; i < length; i++)
        {
            key += ALPHABET[rand() % ALPHABET.length()];
        }
        return key;
    }

    double calculateFitness(string key)
    {
        Cipher cipher(key);
        string plaintext = "HELLO";
        string ciphertext = cipher.encrypt(plaintext);
        int fitness = 0;
        for (int i = 0; i < plaintext.length(); i++)
        {
            if (ciphertext[i] == plaintext[i])
            {
                fitness++;
            }
        }
        return ((double)fitness / plaintext.length());
    }

    string crossover(string key1, string key2)
    {
        string child = "";
        int midpoint = rand() % key1.length();
        for (int i = 0; i < key1.length(); i++)
        {
            if (i < midpoint)
            {
                child += key1[i];
            }
            else
            {
                child += key2[i];
            }
        }
        return child;
    }

    string mutate(string key)
    {
        string newKey = "";
        for (int i = 0; i < key.length(); i++)
        {
            if ((double)rand() / RAND_MAX < MUTATION_RATE)
            {
                newKey += ALPHABET[rand() % ALPHABET.length()];
            }
            else
            {
                newKey += key[i];
            }
        }
        return newKey;
    }

    string run()
    {
        vector<string> population;
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            population.push_back(generateRandomKey(TARGET.length()));
        }

        for (int generation = 0; generation < MAX_GENERATIONS; generation++)
        {
            // Evaluate fitness of each individual in the population
            vector<pair<string, double>> fitnessScores;
            for (int i = 0; i < POPULATION_SIZE; i++)
            {
                double fitness = calculateFitness(population[i]);
                fitnessScores.push_back(make_pair(population[i], fitness));
            }
            // Sort population by fitness in descending order
            sort(fitnessScores.begin(), fitnessScores.end(), [](pair<string, double> &a, pair<string, double> &b)
                 { return a.second > b.second; });

            // Check if the target string has been reached
            if (fitnessScores[0].second == 1.0)
            {
                return fitnessScores[0].first;
            }

            // Select parents for crossover using tournament selection
            vector<string> parents;
            for (int i = 0; i < POPULATION_SIZE / 2; i++)
            {
                string parent1, parent2;
                for (int j = 0; j < TOURNAMENT_SIZE; j++)
                {
                    int index = rand() % POPULATION_SIZE;
                    if (j == 0 || fitnessScores[index].second > calculateFitness(parent1))
                    {
                        parent1 = fitnessScores[index].first;
                    }
                    index = rand() % POPULATION_SIZE;
                    if (j == 0 || fitnessScores[index].second > calculateFitness(parent2))
                    {
                        parent2 = fitnessScores[index].first;
                    }
                }
                parents.push_back(parent1);
                parents.push_back(parent2);
            }

            // Perform crossover and mutation to create new generation
            vector<string> newPopulation;
            for (int i = 0; i < POPULATION_SIZE; i += 2)
            {
                string child = crossover(parents[i], parents[i + 1]);
                child = mutate(child);
                newPopulation.push_back(child);
                newPopulation.push_back(mutate(crossover(parents[i + 1], parents[i])));
            }

            // Replace old population with new generation
            population = newPopulation;

            // Return the best key found after the maximum number of generations
            return fitnessScores[0].first;
        }
    }
};

int main()
{
    srand(time(NULL));
    GeneticAlgorithm ga;
    string key = ga.run();

    cout << "Optimized key: " << key << endl;

    Cipher cipher(key);

    string plaintext = "HELLO";
    string ciphertext = cipher.encrypt(plaintext);
    string decryptedText = cipher.decrypt(ciphertext);

    cout << "Plaintext: " << plaintext << endl;
    cout << "Ciphertext: " << ciphertext << endl;
    cout << "Decrypted text: " << decryptedText << endl;

    return 0;
};