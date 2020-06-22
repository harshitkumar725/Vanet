//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef APPLICATION_CAAPPL_H_
#define APPLICATION_CAAPPL_H_
#include "BasicAppl.h"

class bufferEntry
{
  public:
    CRL_Piece *piece;
    int RSU;
    bool sent;
};

class CAAppl : public BasicAppl{
public:
    CAAppl();
    virtual ~CAAppl();
    virtual void finish();
        virtual void initialize(int);
        virtual int numInitStages() const
        {
            return 1;
        }


        /** @brief handle self messages */
        void handleSelfMsg(cMessage* msg) ;


      private:
        int dataToRSU;
        int InitialWait;
        int CRLsize;
        bool EnableShuffle;
        int pad;   // how many padding are added

        cMessage *Timer1;
        cMessage *Timer2;



        std::ostringstream CRLbytes;
        std::vector<CRL_Piece *> PiecesCRL;
        std::vector<bufferEntry *> buffer;

        void CalculateMatrixA();

        void createCRL();

        void processCRL(std::vector<Certificate *>);
        std::vector<std::string> NOerasure();
        std::vector<CRL_Piece *> addHeader(std::vector<std::string>);

        std::vector<CRL_Piece *> shuffle(std::vector<CRL_Piece *>);
        void fillBuffer(std::vector<CRL_Piece *>);
        void sendBuffer();

};

#endif /* APPLICATION_CAAPPL_H_ */
