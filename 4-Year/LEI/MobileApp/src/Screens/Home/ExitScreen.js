
import React, { useEffect } from 'react';

import { deleteUser} from '../../utils'

import { View, Text } from 'react-native';



export default function ExitScreen({navigation}){

    useEffect(() => {
            deleteUser()
            .then(() => {navigation.navigate('AuthLoading')})
    },[])

    return( <View>
                <Text>
                exit
                </Text>             
            </View>
    );

}
