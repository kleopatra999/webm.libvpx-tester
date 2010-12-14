#include "vpxt_test_declarations.h"

int test_undershoot(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Undershoot";
    char *MyDir = "test_undershoot";

    if (!(argc == 6 || argc == 5))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        tprintf(PRINT_STD,
                "\n\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    string UnderShoot10 = CurTestDirStr;
    UnderShoot10.append(slashCharStr());
    UnderShoot10.append(MyDir);
    UnderShoot10.append("_compression_10.ivf");

    string UnderShoot100 = CurTestDirStr;
    UnderShoot100.append(slashCharStr());
    UnderShoot100.append(MyDir);
    UnderShoot100.append("_compression_100.ivf");

    /////////////OutPutfile////////////
    string TextfileString = CurTestDirStr;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == COMP_ONLY || TestType == TEST_AND_COMP)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (TestType == TEST_AND_COMP)
        print_header_full_test(argc, argv, MainTestDirChar);

    if (TestType == COMP_ONLY)
        print_header_compression_only(argc, argv, MainTestDirChar);

    if (TestType == TEST_ONLY)
        print_header_test_only(argc, argv, CurTestDirStr);

    vpxt_cap_string_print(PRINT_BTH, "%s", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n", argv[argc-1]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.under_shoot_pct = 10;

        if (vpxt_compress_ivf_to_ivf(input, UnderShoot10.c_str(), speed, BitRate, opt, CompressString, 10, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.under_shoot_pct = 100;

        if (vpxt_compress_ivf_to_ivf(input, UnderShoot100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n");

    tprintf(PRINT_BTH, "\n");
    long FileIndexOutputCharbytes = vpxt_file_size(UnderShoot10.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    long File2bytes = vpxt_file_size(UnderShoot100.c_str(), 1);
    tprintf(PRINT_BTH, "\n");

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (FileIndexOutputCharbytes < File2bytes)
    {
        vpxt_formated_print(RESPRT, "File size 1:%i < File size 2:%i - Passed", FileIndexOutputCharbytes , File2bytes);

        tprintf(PRINT_BTH, "\n\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    if (FileIndexOutputCharbytes == File2bytes)
    {
        vpxt_formated_print(RESPRT, "No effect try different file - Indeterminate");

        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }
    else
    {
        vpxt_formated_print(RESPRT, "File size 1:%i > File size 2:%i - Failed", FileIndexOutputCharbytes , File2bytes);

        tprintf(PRINT_BTH, "\n\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}