#ifndef OUTPUT_LISTNER_INTERFACE_HPP
#define OUTPUT_LISTNER_INTERFACE_HPP

namespace core {
    /**
     * @brief a Listner 
     * @details and output listner that allows libraries to communicate with the apllication 
     */
    class OutputListner {
            public :
                /**
                 * @brief enter an info message 
                 * @param[in] input message event
                 */
                virtual void onInfo (const char *aMessaage) {
                };
                /**
                 * @brief enter an debug message
                 * @param[in] input message event 
                 */
                virtual void onDebug (const char *aMessaage) {
                };
                /**
                 * @brief enter a warning message 
                 * @param[in] input message event 
                 */
                virtual void onWarning (const char *aMessaage) {
                };
                /**
                 * @brief enter an error message 
                 * @param[in] input message event
                 */
                virtual void onError (const char *aMessaage) {
                };
                /**
                 * @brief enter an error message 
                 * @param[in] input message event
                 */
                virtual void onTrace (const char *aMessaage) {
                };

                virtual ~OutputListner (){}
    };
}
#endif 