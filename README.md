# project-uoa


Contributors:   Σωτήρης Χρυσάνθου   ΑΜ: 1115201700224
                Ιωάννης Ιωαννίδης   ΑΜ: 1115201600216

Data structures:
    Hashtable for folders(sites) and more Hashtables for each folder's files.
    Each item uses lists for accessing its common items and specs and a hashtable for its uncommon items. The hashtable contais the common lists of the uncommon items (one for each group).
    The vocabulary is stored in a hashtable and each item contains an array of the same size with its tfidf or bow values. The correct position for each word is stored inside the vocabulary hashtable.
    Many structures were already implemented by us during that course and modified for satisfying this project's requirements.
    
The program can be used to train only, test only or both. for testing only, the "best_w" file generated by this program must be present in the directory.
This part uses threads for trainig the data (using mini batching) as well as testing them.
The value of a and the threshold are vocates inside the training functions in src/functions.cpp with values we found worked best in the examples given.
The number of threads, batch sizeand number of repetitions are given to the training funcion as arguments. We notice that after a certain number of repetitions the results for 0 and 1 stop showing any difference. The number we found to be best was 10 repetitions. As for the theads and batch size, we tested many variations, but with a large batch, weights chane less often, giving worse results.

    Usage:
    ./exe -p 2 -d ./data/2013_camera_specs -c ./data/sigmod_large_labelled_dataset.csv -v tfidf -t train -o output_file.txt
    -p  : which part to execute 1 or 2 (a default is set inside main)
    -d  : data folder
    -c  : csv file
    -v  : tfidf or bow (set to tfidf by default)
    -t  : train, test or both (both is set as default)
            for testing only, the best weights file must be present in the current directory with the name "best_W"
    -o  : the output file for common items (stdout is set as default)

Notes:
    During this project a fully functional JSON file parser was implemented.



git commands help: https://confluence.atlassian.com/bitbucketserver/basic-git-commands-776639767.html
