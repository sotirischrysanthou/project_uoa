# project-uoa


Contributors:   Σωτήρης Χρυσάνθου   ΑΜ: 1115201700224
                Ιωάννης Ιωαννίδης   ΑΜ: 1115201600216

Data structures:
    Hashtable for folders(sites) and more Hashtables for each folder's files.
    Each item uses lists for accessing its common items and specs and a hashtable for its uncommon items. The hashtable contais the common lists of the uncommon items (one for each group).
    The vocabulary is stored in a hashtable and each item contains an array of the same size with its tfidf or bow values. The correct position for each word is stored inside the vocabulary hashtable.
    Many structures were already implemented by us during that course and modified for satisfying this project's requirements.

    ./exe -p 2 -d ./data/2013_camera_specs -c ./data/sigmod_large_labelled_dataset.csv -v tfidf -t both -o output_file.txt
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
