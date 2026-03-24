#ifndef GlobalDataManager_hh
#define GlobalDataManager_hh 1

#include <iostream>

class GlobalDataManager
{
    private:
        static GlobalDataManager* instance;
        int n_li7_recoil_phonon;
        int li7_track_id;
        double Etot_li7_recoil_phonon;

        GlobalDataManager();
    
    public:


        void RGlobalDataManager(const GlobalDataManager&) = delete;
        GlobalDataManager& operator=(const GlobalDataManager&) = delete;
        
        static GlobalDataManager* GetInstance();

        int GetLi7RecoilPhononNumber() const ;
        void AddOnePhonon(); 

        int GetLi7RecoilTrackID() const ;
        void SetLi7TrackID(int trackid) ;

        double GetLi7RecoilPhononEnergy() const;
        void AddLi7RecoilPhononEnergy(double E);



        void ResetVariables();
        
        ~GlobalDataManager();  // DestructoresetVariable() { n_li7_recoil_photon = 0; li7_track_id = -1;}

};



#endif